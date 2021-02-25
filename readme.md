
## IIT Test assessment as "robotic software engineer"

### intro:

This excercise templates the hello-world example from the original DDS-exproxima repository.<br>


 Two single processes are assumed in the canonical publisher/subscriber paradigm where a custom message has been created (MapElemet.idl --> automatic code-gen has been than used for generating the message and topic code). <br>

 Improvements are minimal w.r.t. the DDS provided example, but a time-stamp has been added to the message sent enhancing latency measurement ( the additional function for t-measurement has been added in the helpers.h header file).





### compile

mkdir build<br>
cd build<br>
cmake ..<br>
make <br>

two executables will be generated.

### run

1. open a terminal and run: ./MapElementSubscriber
2. open a second terminal and run: ./MapElementPublisher


 I thank you in advance for the time spent reviewing my test.

