#include "PipelineElements/ExternalInput.hpp"
#include <yaml-cpp/yaml.h>
#include <exception>
#include "Logging/ILogging.hpp"
#include "ImageContainer/ImageContainer.hpp"
#include "ImageContainer/ImagePool.hpp"
#include "PipelineElements/IGenericSink.hpp"

using namespace FIPP::pipe;
using namespace std;

ExternalInput::ExternalInput(YAML::Node config, int elemId, std::shared_ptr<FIPP::logging::ILogger> log)
{
    this->m_log = log;
    this->m_uuid = elemId;
    this->m_config = FIPP::img::getContainerConfigFromYaml(config["imgConfig"]);
    this->m_pool = std::make_unique<FIPP::img::ImagePool>(10, this->m_config);
}
ExternalInput::~ExternalInput()
{
    if (m_successor)
    {
        ElementState succState = m_successor->getState();
        if (succState == ElementState::RUNNING || succState == ElementState::PAUSED || succState == ElementState::STARTING)
        {
            m_successor->stopElement();
            while (m_successor->getState() == ElementState::STOPPING)
            {
            };
        }
    }
}

std::shared_ptr<FIPP::img::ImageContainer> ExternalInput::getImgContainer()
{
    return this->m_pool->getNextFreeImage();
}

void ExternalInput::pushImgToPipe(std::shared_ptr<FIPP::img::ImageContainer> img)
{
    if (m_successor == nullptr)
    {
        throw std::runtime_error("Successor not set!");
    }
    if (m_successor->getState() == ElementState::RUNNING)
    {
        m_successor->addImageToInputPipe(img);
    }
    else
    {
        throw std::runtime_error("Successor not in running state!");
    }
}

void ExternalInput::connectSuccessor(std::shared_ptr<IGenericSink> elem)
{
    this->m_successor = elem;
}

bool ExternalInput::interogateConnection()
{
    return this->m_successor->interogateConnection(this->m_config, this->m_uuid);
}

StartState ExternalInput::startElement(int predecessorId)
{
    this->m_state = ElementState::RUNNING;
    return this->m_successor->startElement(this->m_uuid);
}

StopState ExternalInput::stopElement()
{
    this->m_state = ElementState::IDLE;
    return this->m_successor->stopElement();
}