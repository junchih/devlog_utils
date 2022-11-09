package main

import (
	"encoding/csv"
	"os"
	"sort"
	"strconv"

	"github.com/spf13/pflag"
	ffmpeg "github.com/u2takey/ffmpeg-go"
)

var (
	videoStart int64
	blankJump  int64
	keyrecFile string
	inputFile  string
	outputFile string
)

func init() {

	pflag.Int64VarP(&videoStart, "start", "S", 0,
		"the UTC timestamp video started at")
	pflag.Int64VarP(&blankJump, "jump", "J", 3,
		"blank time to jump")
	pflag.StringVarP(&keyrecFile, "keyrec", "R", "./keyrec.csv",
		"keyrec.csv file")
	pflag.StringVarP(&inputFile, "input", "I", "",
		"input video file")
	pflag.StringVarP(&outputFile, "output", "I", "",
		"output video file")

	pflag.Parse()
}

func main() {

	keyrec, err := os.Open(keyrecFile)
	if err != nil {
		panic(err)
	}
	defer keyrec.Close()

	csvReader := csv.NewReader(keyrec)
	csvReader.Comma = '\t'
	records, err := csvReader.ReadAll()
	if err != nil {
		panic(err)
	}
	records = records[1:]

	timestamps := make([]int64, len(records))
	for i, record := range records {
		secStr := record[0]
		msecStr := record[1]
		sec, err := strconv.ParseInt(secStr, 10, 64)
		if err != nil {
			panic(err)
		}
		msec, err := strconv.ParseInt(msecStr, 10, 64)
		if err != nil {
			panic(err)
		}
		timestamps[i] = (sec * 1000) + (msec / 1000)
	}

	cuts := []int64{}
	for i := 0; i < len(timestamps)-1; i++ {
		blank := timestamps[i+1] - timestamps[i]
		if blank > blankJump*1000 {
			cuts = append(cuts, timestamps[i], timestamps[i+1])
		}
	}

	ind := sort.Search(
		len(cuts),
		func(i int) bool { return cuts[i] >= videoStart },
	)
	cuts = cuts[ind:]
	if ind%2 != 0 {
		cuts = append([]int64{videoStart}, cuts...)
	}

	stream := ffmpeg.Input(inputFile).Output(outputFile)
	for i := 0; i < len(cuts); i += 2 {
		stream = stream.Filter("select", nil)
		// https://stackoverflow.com/questions/50594412/cut-multiple-parts-of-a-video-with-ffmpeg
	}

	err = stream.Run()
	if err != nil {
		panic(err)
	}
}
