cd ..
if exist build == FALSE
	md build
cd build && cmake ..