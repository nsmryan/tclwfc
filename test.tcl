load "./tclwfc.so"

set fileName wfc/samples/cave.png

puts open
tclwfc::wfc 32 32 $fileName 3 3 1 1 1 1

puts gen
set pixels [tclwfc::generate]

puts pixels
puts $pixels

