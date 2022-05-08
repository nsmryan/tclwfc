package require Tk

lappend auto_path build
package require tclwfc

set fileName wfc/samples/curl.png

set width 20
set height 20

proc dictGetDefault { dictionary key value } {
    if { [dict exists $dictionary $key] } { 

        return [dict get $dictionary $key]
    }
    return $value
}

proc wfc { name args } {
    if { [info proc $name] != "" } {

        destroyWfc $name
    }

    set width [dictGetDefault $args -width 20]
    set height [dictGetDefault $args -height 20]
    set fileName [dictGetDefault $args -fileName data/wfc_seed_2.png]
    set tileWidth [dictGetDefault $args -tileWidth 3]
    set tileHeight [dictGetDefault $args -tileHeight 3]
    set expandInput [dictGetDefault $args -expandInput 1]
    set addHoriz [dictGetDefault $args -addHoriz 1]
    set addVert [dictGetDefault $args -addVert 1]
    set add90DegreeRotations [dictGetDefault $args -add90DegreeRotations 1]

    tclwfc::wfc $name $width $height $fileName $tileWidth $tileHeight $expandInput $addHoriz $addVert $add90DegreeRotations 
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

proc timeit { cmd } {
    set start [clock microseconds]
    {*}$cmd
    set end [clock microseconds]
    puts "took [expr ($end - $start) / 1000000.0]"
}

proc redraw { { ms 0 } } {
    draw [gen]

    if { $ms > 0 } {
        after $ms [list redraw $ms]
    }
}

wfc gen $fileName -width $width -height $height

