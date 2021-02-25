
### IIT Test assessment as "robotic software engineer"

## intro:

This excercise templates the hello-world example from the original DDS-exproxima repository.<br>


 Two single processes are assumed in the canonical publisher/subscriber paradigm where a custom message has been created (MapElemet.idl --> automatic code-gen has been than used for generating the message and topic code)


## compile

mkdir build
cd build
cmake ..
make 

two executables will be generated.

## run

1. open a terminal and run: ./MapElementSubscriber
2. open a second terminal and run: ./MapElementPublisher

