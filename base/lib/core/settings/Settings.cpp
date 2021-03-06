// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <core/settings/Settings.hpp>
#include <core/settings/SettingsPresenter.hpp>
#include <core/settings/SettingsView.hpp>
#include <iscore/plugins/settingsdelegate/SettingsDelegateFactory.hpp>
#include <iscore/plugins/settingsdelegate/SettingsDelegateModel.hpp>
#include <iscore/plugins/settingsdelegate/SettingsDelegateView.hpp>

namespace iscore
{
Settings::Settings()
    : m_settingsView{new SettingsView(nullptr)}
    , m_settingsPresenter{new SettingsPresenter(m_settingsView, nullptr)}
{
}

Settings::~Settings()
{
  m_settingsView->deleteLater();
  for (auto& ptr : m_settings)
  {
    auto p = ptr.release();
    p->deleteLater();
  }
}

void Settings::setupSettingsPlugin(
    QSettings& s,
    const iscore::ApplicationContext& ctx,
    SettingsDelegateFactory& plugin)
{
  auto model = plugin.makeModel(s, ctx);
  if (!model)
    return;

  auto& model_ref = *model;
  m_settings.push_back(std::move(model));

  auto view = plugin.makeView();
  if (!view)
    return;

  auto pres = plugin.makePresenter(model_ref, *view, m_settingsPresenter);
  if (pres)
  {
    // Ownership transfer
    m_settingsPresenter->addSettingsPresenter(pres);
    m_settingsView->addSettingsView(view);
  }
  else
  {
    delete view;
  }
}
}
