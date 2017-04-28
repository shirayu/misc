package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	scanner := bufio.NewScanner(os.Stdin)

	countmap := map[string]int{}

	expected_line_num := 0
	if len(os.Args) >= 2 {
		_expected_line_num, err := strconv.Atoi(os.Args[1])
		if err == nil {
			expected_line_num = _expected_line_num
		}
	}

	for lid := 0; scanner.Scan(); lid++ {
		for _, word := range strings.Split(scanner.Text(), " ") {
			countmap[word] += 1
		}
		if lid%100 == 0 {
			if expected_line_num == 0 {
				fmt.Fprintf(os.Stderr, "\r%d", lid)
			} else {
				fmt.Fprintf(os.Stderr, "\r%d %.3f%%", lid, float64(lid)/float64(expected_line_num)*100)
			}
		}
	}

	for word, count := range countmap {
		fmt.Printf("%s\t%d\n", word, count)
	}
}
