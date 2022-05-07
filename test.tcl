package require Tk

lappend auto_path build
package require tclwfc

set fileName wfc/samples/curl.png

set width 32
set height 32

proc wfc { name fileName } {
    global width height

    if { [info proc $name] != "" } {
        destroyWfc $name
    }

    tclwfc::wfc $name $width $height $fileName 3 3 1 1 1 1
}

proc destroyWfc { name } {
    rename $name ""
}

canvas .canvas 
pack .canvas -expand yes -fill both
update

bind . <Destroy> exit

set canvasWidth [winfo width .canvas]
set canvasHeight [winfo height .canvas]

set cellWidth [expr $canvasWidth / $width]
set cellHeight [expr $canvasHeight / $height]

set cellWidth [expr min($cellWidth, $cellHeight)]
set cellHeight [expr min($cellWidth, $cellHeight)]

proc draw { pixels } {
	global width height cellWidth cellHeight 

	.canvas delete all
	for { set y 0 } { $y < $width } { incr y } {
		for { set x 0 } { $x < $width } { incr x } {
			set xPixel [expr $x * $cellWidth]
			set yPixel [expr $y * $cellHeight]
			set xPixelExtent [expr $xPixel + $cellWidth]
			set yPixelExtent [expr $yPixel + $cellHeight]

			set index [expr $y * $width + $x]
			set value [expr 0xFFFFFF & [lindex $pixels $index]]
			set color #[format %06X $value]
			.canvas create rectangle $xPixel $yPixel $xPixelExtent $yPixelExtent -fill $color
		}
	}
}

proc redraw { } {
	draw [gen]
}

wfc gen $fileName
