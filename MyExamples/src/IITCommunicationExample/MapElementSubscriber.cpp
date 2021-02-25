#include "MapElementPubSubTypes.h"
#include "helpers.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <map>


using namespace eprosima::fastdds::dds;


class MapElementSubscriber
{
private:

    DomainParticipant* participant_;

    Subscriber* subscriber_;

    DataReader* reader_;

    Topic* topic_;

    TypeSupport type_;


    class SubListener : public DataReaderListener
    {
    public:

        SubListener()
            : samples_(0)
        {
        }

        ~SubListener() override
        {
        }

        void on_subscription_matched(
                DataReader*,
                const SubscriptionMatchedStatus& info) override
        {
            if (info.current_count_change == 1)
            {
                std::cout << "Subscriber matched." << std::endl;
            }
            else if (info.current_count_change == -1)
            {
                std::cout << "Subscriber unmatched." << std::endl;
            }
            else
            {
                std::cout << info.current_count_change
                        << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
            }
        }

        
      

        void on_data_available(
                DataReader* reader) override
        {
        
            // receive stream
            SampleInfo info;
            if (reader->take_next_sample(&map_element_, &info) == ReturnCode_t::RETCODE_OK)
            {
                if (info.valid_data)
                {
                    if(map_element_.key() == "Single-Init-Message" && map_element_.value()==0.0)
                    {
                      std::cout << "key: " << map_element_.key()   
                                << " value: " << map_element_.value() 
                                << std::endl
                                << "Single message received, streaming started..." 
                                <<std::endl;
                    }
                    else
                    {  

                        samples_++;
                        std::cout << "key: " << map_element_.key() 
                                  << " value: " << map_element_.value()
                                  << " t-received: " << millis()
                                  << " t-sent: " << map_element_.tstamp() 
                                  << " latency[ns]: " << millis() - map_element_.tstamp()
                                  << " RECEIVED." << std::endl;

                        performances_[map_element_.key()] =  millis() - map_element_.tstamp();
                    }
                }
            }
        }

        //! Getter for the perfomances map
        std::map<std::string,double> get_performances_map()
        {
            return performances_;
        }

        MapElement map_element_;

        std::atomic_int samples_;

        std::map <std::string,double> performances_;


    } listener_;

public:

    MapElementSubscriber()
        : participant_(nullptr)
        , subscriber_(nullptr)
        , topic_(nullptr)
        , reader_(nullptr)
        , type_(new MapElementPubSubType())
    {
    }

    virtual ~MapElementSubscriber()
    {
        if (reader_ != nullptr)
        {
            subscriber_->delete_datareader(reader_);
        }
        if (topic_ != nullptr)
        {
            participant_->delete_topic(topic_);
        }
        if (subscriber_ != nullptr)
        {
            participant_->delete_subscriber(subscriber_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    //!Initialize the subscriber
    bool init()
    {
        DomainParticipantQos participantQos;
        participantQos.name("Participant_subscriber");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);

        if (participant_ == nullptr)
        {
            return false;
        }

        // Register the Type
        type_.register_type(participant_);

        // Create the subscriptions Topic
        topic_ = participant_->create_topic("MapElementTopic", "MapElement", TOPIC_QOS_DEFAULT);

        if (topic_ == nullptr)
        {
            return false;
        }

        // Create the Subscriber
        subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);

        if (subscriber_ == nullptr)
        {
            return false;
        }

        // Create the DataReader
        reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, &listener_);

        if (reader_ == nullptr)
        {
            return false;
        }

        return true;
    }

    //!Run the Subscriber
    void run(
        uint32_t samples)
    {

        while(listener_.samples_ < samples)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10000));

        }
    }

    //! Recap performances:
    void recap()
    {
        std::cout << "summarizing performances..." <<std::endl;
        std::map<std::string,double> performances = listener_.get_performances_map();

        double tot_lat =0;
        int cnt =0;
        for(auto pe : performances)
        {
            cnt++;
            std::cout<< pe.first << " : " <<pe.second << std::endl;
            tot_lat += pe.second;
        }

        std::cout << "average latency: " << tot_lat/cnt <<  " [ns] " <<std::endl;


    }
};

int main(
        int argc,
        char** argv)
{
    std::cout << "Starting subscriber." << std::endl;
    int samples = 10;

    MapElementSubscriber* example_sub = new MapElementSubscriber();
    if(example_sub->init())
    {
        example_sub->run(static_cast<uint32_t>(samples));
        example_sub->recap();
    }

    delete example_sub;
    return 0;
}
