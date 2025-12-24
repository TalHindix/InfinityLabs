#include "reactor.hpp"
using namespace ilrd;

#define TIMEOUT (10)

class ListenSelect : public Reactor::IListener
{
public:
    std::vector<Reactor::FdPair> Listen(const std::vector<Reactor::FdPair>& set) override
    {
        struct timeval timeout{};
        timeout.tv_sec = TIMEOUT;
        m_vector = set;

        VectorToSet(set);
        if (0 > select(m_fd_max + 1, &m_set, NULL, NULL, &timeout))
        {
            throw std::runtime_error(std::string("select()"));
        }
        SetToVector();

        return m_vector;
    }

private:
    std::vector<Reactor::FdPair> m_vector;
    fd_set m_set;
    int m_fd_max;

    void VectorToSet(const std::vector<Reactor::FdPair>& set);
    void SetToVector();
};

void ListenSelect::VectorToSet(const std::vector<Reactor::FdPair>& set)
{
    FD_ZERO(&m_set);
    m_fd_max = -1;

    for (Reactor::FdPair fd : set)
    {
        if (fd.first >= 0)
        {
            FD_SET(fd.first, &m_set);
            if (fd.first > m_fd_max) { m_fd_max = fd.first; }
        }
    }
}

void ListenSelect::SetToVector()
{
    std::vector<Reactor::FdPair> fds;
    fds.reserve(m_fd_max + 1);

    for (const Reactor::FdPair& key : m_vector)
    {
        if (FD_ISSET(key.first, &m_set)) {fds.push_back(key);}
    }

    m_vector = fds;
}
