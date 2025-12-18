
 #include <iostream> //std::cout
 #include <string>   //std::string

 #include "dispatcher.hpp"

 using namespace ilrd;
 struct Event
 {
    std::string data;
    int id;
 };

 //Observer
 class SimpleReceiver
 {
 public:
    SimpleReceiver() : m_lastData(""), m_notify_count(0)
    {

    }

    void OnRecieve(const Event& event)
    {
        ++m_notify_count;
        m_lastData = event.data;

        std::cout<<" [SimpleReciever] Recieved: " << event.data << " (id = " << event.id << ")\n";
    }

    void OnDeath()
    {
        std::cout << " [Simple Reciever] Dispatcher was Destroyed. \n";
    }

    int GetCount() const
    {
        return m_notify_count;
    }

    const std::string& GetLastData() const
    {
        return m_lastData;
    }

 private:
    std::string m_lastData;
    int m_notify_count;
 };


 class SimpleSender
 {
 public:
    void SendEvent(const std::string& msg, int id)
    {
        Event ev = {msg , id};
        m_dispatcher.Broadcast(ev);
    }

    Dispatcher<Event>& GetDispatcher()
    {
        return m_dispatcher;
    }



 private:
    Dispatcher<Event> m_dispatcher;
 };

 void PrintTestResult(const std::string& testName, bool isPass)
 {
    std::cout << (isPass ? "[PASS] ": "[FAIL] ") << testName << std::endl;
 }

 void TestOneSubscriber()
 {
    std::cout<< "\n=== Test 1: One Receiver ===\n";

    SimpleSender sender;
    SimpleReceiver reciever;

    Callback<Event,SimpleReceiver> callback(reciever, &SimpleReceiver::OnRecieve, &SimpleReceiver::OnDeath);

    sender.GetDispatcher().Subscribe(&callback);
    sender.SendEvent("Data for test 1 ", 1);

    bool testPassed = (reciever.GetCount() == 1) &&
                      (reciever.GetLastData() == "Data for test 1 ");

    PrintTestResult("One subscriber receives exactly one event" , testPassed);

 }

 int main()
 {
    TestOneSubscriber();
    return 0;
 }