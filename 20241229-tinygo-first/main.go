package main

import (
	"fmt"
	"machine"
	"time"
)

func main() {
	led := machine.GP14
	led.Configure(machine.PinConfig{
		Mode: machine.PinOutput,
	})
	n := 0
	for {
		fmt.Printf("OFF! %d\r\n", n)
		led.Low()
		time.Sleep(time.Second)
		fmt.Printf("ON! %d\r\n", n)
		led.High()
		time.Sleep(time.Second)
		n++
	}
}
