#include "MapElementPubSubTypes.h"
#include "helpers.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

#include <string>// dictionary's key
#include <random>// random value
#include <stdlib.h>

using namespace eprosima::fastdds::dds;


class MapElementPublisher
{
private:

    MapElement map_el_;

    DomainParticipant* participant_;

    Publisher* publisher_;

    Topic* topic_;

    DataWriter* writer_;

    TypeSupport type_;


    class PubListener : public DataWriterListener
    {
    public:

        PubListener()
            : matched_(0)
        {
        }

        ~PubListener() override
        {
        }

        void on_publication_matched(
                DataWriter*,
                const PublicationMatchedStatus& info) override
        {
            if (info.current_count_change == 1)
            {
                matched_ = info.total_count;
                std::cout << "Publisher matched." << std::endl;
            }
            else if (info.current_count_change == -1)
            {
                matched_ = info.total_count;
                std::cout << "Publisher unmatched." << std::endl;
            }
            else
            {
                std::cout << info.current_count_change
                        << " is not a valid value for PublicationMatchedStatus current count change." << std::endl;
            }
        }

        std::atomic_int matched_;

    } listener_;

public:

    MapElementPublisher()
        : participant_(nullptr)
        , publisher_(nullptr)
        , topic_(nullptr)
        , writer_(nullptr)
        , type_(new MapElementPubSubType())
    {
    }

    virtual ~MapElementPublisher()
    {
        if (writer_ != nullptr)
        {
            publisher_->delete_datawriter(writer_);
        }
        if (publisher_ != nullptr)
        {
            participant_->delete_publisher(publisher_);
        }
        if (topic_ != nullptr)
        {
            participant_->delete_topic(topic_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    //!Initialize the publisher
    bool init()
    {
        map_el_.tstamp(0);
        map_el_.key("start");
        map_el_.value(.0);

        DomainParticipantQos participantQos;
        participantQos.name("Participant_publisher");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);

        if (participant_ == nullptr)
        {
            return false;
        }

        // Register the Type
        type_.register_type(participant_);

        // Create the publications Topic
        topic_ = participant_->create_topic("MapElementTopic", "MapElement", TOPIC_QOS_DEFAULT);

        if (topic_ == nullptr)
        {
            return false;
        }

        // Create the Publisher
        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);

        if (publisher_ == nullptr)
        {
            return false;
        }

        // Create the DataWriter
        writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT, &listener_);

        if (writer_ == nullptr)
        {
            return false;
        }
        return true;
    }


    //Send single message
    bool publish_single()
    {   
        
        if(listener_.matched_>0)
        {
            map_el_.tstamp(millis());
            map_el_.key("Single-Init-Message");
            map_el_.value((double) 0.0);
            writer_->write(&map_el_);
            return true;
        }
        

        return false;
    }

    //Stream messages
    bool publish_stream(int sample)
    {
        if (listener_.matched_ > 0)
        {
            map_el_.tstamp(millis());// include timestamp in the msg
            map_el_.key("msg=>"+std::to_string(sample));// change msg key
            map_el_.value((double) (rand() %100));// create random number
            writer_->write(&map_el_);
            return true;
        }
        return false;
    }

    //!Run the Publisher
    void run(
            uint32_t samples)
    {

        // send single message
        bool ret = false;
        while(!ret)
        {
            ret = publish_single();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
       

        // stream multiple messages
        uint32_t samples_sent = 0;
        while (samples_sent < samples)
        {
            if (publish_stream(samples_sent))
            {
                samples_sent++;
                std::cout << "key: " << map_el_.key() 
                          << " value: " << map_el_.value()
                          << " @time: " << map_el_.tstamp()
                          << " SENT" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
};

int main(
        int argc,
        char** argv)
{
    std::cout << "Starting publisher." << std::endl;
    int samples = 10;

    MapElementPublisher* example_pub = new MapElementPublisher();
    if(example_pub->init())
    {
        example_pub->run(static_cast<uint32_t>(samples));
    }

    delete example_pub;
    return 0;
}