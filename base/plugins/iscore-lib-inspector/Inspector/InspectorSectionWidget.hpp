#pragma once
#include <iscore_lib_inspector_export.h>
#include <iscore/widgets/MarginLess.hpp>

#include <QString>
#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>
class QMenu;

namespace Inspector
{
class ISCORE_LIB_INSPECTOR_EXPORT MenuButton final : public QPushButton
{
  public:
    MenuButton(QWidget* parent);
};

/** @brief InspectorSectionWidget is widget that can fold or unfold his
 * content.
 *
 * A header with a name is always displayed.
 * It contains one main widget in a QScrollArea with a vertical layout, that
 * can be folded/unfolded on click on the arrow button.
 */
class ISCORE_LIB_INSPECTOR_EXPORT InspectorSectionWidget : public QWidget
{
  Q_OBJECT
public:
  explicit InspectorSectionWidget(
      bool editable = false,
        QWidget* parent = nullptr);
  InspectorSectionWidget(
      QString name,
      bool nameEditable = false,
      QWidget* parent = nullptr);
  virtual ~InspectorSectionWidget();

  QMenu* menu() const
  {
    return m_menu;
  }
  QString name() const;

  // Display tool
  void expand(bool b);

  // Manage section
  //! change the name in the header
  void renameSection(QString newName);

  //! add the widget newWidget in the main layout
  void addContent(QWidget* newWidget);

  //! removes the widget from the main layout
  void removeContent(QWidget* toRemove);

  void removeAll();
  void showMenu(bool b);

signals:
  void nameChanged(QString newName);

private:
  iscore::MarginLess<QVBoxLayout> m_generalLayout; /*!< main layout */

  QWidget m_title;
  iscore::MarginLess<QHBoxLayout> m_titleLayout;

  QToolButton m_unfoldBtn; /*!< button for the fold/unfold action */

  QPushButton m_buttonTitle;
  QLineEdit m_sectionTitle; /*!< header label editable ? */
  MenuButton m_menuBtn;
  QMenu* m_menu{};

  bool m_isUnfolded{};
};
}
