set breakpoint pending on

set print thread-events off
set debug threads off
set non-stop off

set listsize 30
set prompt => 
break Error.h:17
run
y
