project(fmtlib)
include_directories(./fmt-3.0.0/fmt)
add_library(fmtlib STATIC
	fmt-3.0.0/fmt/format.cc
	fmt-3.0.0/fmt/ostream.cc
	fmt-3.0.0/fmt/posix.cc
	fmt-3.0.0/fmt/format.h
	fmt-3.0.0/fmt/ostream.h
	fmt-3.0.0/fmt/posix.h
	fmt-3.0.0/fmt/time.h
)
