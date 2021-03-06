// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "LibraryPanelFactory.hpp"
#include "LibraryPanelDelegate.hpp"

namespace Library
{

std::unique_ptr<iscore::PanelDelegate>
PanelDelegateFactory::make(const iscore::GUIApplicationContext& ctx)
{
  return std::make_unique<PanelDelegate>(ctx);
}
}
