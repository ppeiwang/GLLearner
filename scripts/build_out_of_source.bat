cd ..
if exist build == FALSE
	md build

cd build && cmake ..

if NOT ["%errorlevel%"]==["0"] pause