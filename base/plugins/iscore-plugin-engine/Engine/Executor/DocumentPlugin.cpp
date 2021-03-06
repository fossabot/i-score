// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <Scenario/Document/ScenarioDocument/ScenarioDocumentModel.hpp>

#include "BaseScenarioComponent.hpp"
#include "DocumentPlugin.hpp"
#include <Engine/Executor/ConstraintComponent.hpp>
#include <Engine/Executor/StateProcessComponent.hpp>
#include <Engine/Executor/Settings/ExecutorModel.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#include <Scenario/Document/BaseScenario/BaseScenario.hpp>
#include <core/document/Document.hpp>
#include <core/document/DocumentModel.hpp>
#include <iscore/plugins/documentdelegate/plugin/DocumentPlugin.hpp>

#include <Engine/ApplicationPlugin.hpp>
namespace Engine
{
namespace Execution
{
DocumentPlugin::DocumentPlugin(
    const iscore::DocumentContext& ctx,
    Id<iscore::DocumentPlugin> id,
    QObject* parent)
    : iscore::DocumentPlugin{ctx, std::move(id),
                             "OSSIADocumentPlugin", parent}
    , m_ctx{
          ctx, m_base,
          ctx.plugin<Explorer::DeviceDocumentPlugin>(),
          ctx.app.interfaces<ProcessComponentFactoryList>(),
          ctx.app.interfaces<StateProcessComponentFactoryList>(),
          {}, {},
          m_editionQueue
      }
    , m_base{m_ctx, this}
{
}

DocumentPlugin::~DocumentPlugin()
{
  if (m_base.active())
  {
    m_base.baseConstraint().stop();
    clear();
  }
}

void DocumentPlugin::reload(Scenario::ConstraintModel& cst)
{
  if (m_base.active())
  {
    m_base.baseConstraint().stop();
  }
  clear();
  auto& ctx = iscore::DocumentPlugin::context();
  auto& settings = ctx.app.settings<Engine::Execution::Settings::Model>();
  m_ctx.time = settings.makeTimeFunction(ctx);
  m_ctx.reverseTime = settings.makeReverseTimeFunction(ctx);
  auto parent = dynamic_cast<Scenario::ScenarioInterface*>(cst.parent());
  ISCORE_ASSERT(parent);
  m_base.init(BaseScenarioRefContainer{cst, *parent});

  runAllCommands();
}

void DocumentPlugin::clear()
{
  if(m_base.active())
  {
    runAllCommands();
    m_base.cleanup();
    runAllCommands();
  }
}

void DocumentPlugin::on_documentClosing()
{
  if (m_base.active())
  {
    m_base.baseConstraint().stop();
    m_ctx.context().doc.app.guiApplicationPlugin<Engine::ApplicationPlugin>().on_stop();
    clear();
  }
}

const BaseScenarioElement& DocumentPlugin::baseScenario() const
{
  return m_base;
}

bool DocumentPlugin::isPlaying() const
{
  return m_base.active();
}

void DocumentPlugin::runAllCommands() const
{
  bool ok = false;
  ExecutionCommand com;
  do {
    ok = m_editionQueue.try_dequeue(com);
    if(ok && com)
      com();
  } while(ok);
}
}
}
