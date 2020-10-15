#include "programmodule.h"
#include <DIconButton>
#include <DLabel>
#include <DPushButton>

#include "widgets/probitwidget.h"

const int EXPRESSIONBAR_HEIGHT = 95;

ProgramModule::ProgramModule(QWidget *parent)
    : DWidget(parent)
    , m_proExpressionBar(new ProExpressionBar(this))
    , m_proListView(new ProListView(this))
    , m_proListModel(new ProListModel(this))
    , m_proListDelegate(new ProListDelegate())
    , m_checkBtnKeypad(new ProCheckBtnKeypad(this))
    , m_programmerKeypad(new ProgrammerKeypad(this))
    , m_proSystemKeypad(new ProSystemKeypad(this))
    , m_stackWidget(new QStackedWidget(this))
    , m_byteArrowRectangle(new ArrowRectangle(DArrowRectangle::ArrowTop, DArrowRectangle::FloatWidget, this))
    , m_byteArrowListWidget(new DListWidget)
    , m_byteProgrammerArrowDelegate(new ProgrammerArrowDelegate)
    , m_shiftArrowRectangle(new ArrowRectangle(DArrowRectangle::ArrowTop, DArrowRectangle::FloatWidget, this))
    , m_shiftArrowListWidget(new DListWidget())
    , m_shiftProgrammerArrowDelegate(new ProgrammerArrowDelegate)
{
    m_proExpressionBar->setFixedHeight(EXPRESSIONBAR_HEIGHT);
    m_proListView->setModel(m_proListModel);
    m_proListView->setItemDelegate(m_proListDelegate);
    m_stackWidget->addWidget(m_programmerKeypad);
    m_stackWidget->addWidget(m_proSystemKeypad);  //此处可继续添加内存界面
    m_stackWidget->setCurrentWidget(m_programmerKeypad);
    m_stackWidget->setFixedSize(451, 279);

    initArrowRectangle();

    QVBoxLayout *vlay = new QVBoxLayout(this);
    vlay->addWidget(m_proExpressionBar);
    vlay->addWidget(m_proListView);
    vlay->addWidget(m_checkBtnKeypad);
    vlay->addWidget(m_stackWidget);
    vlay->setSpacing(0);
    vlay->setMargin(0);
    vlay->setContentsMargins(0, 0, 0, 0);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            this, &ProgramModule::checkBtnKeypadThemeChange);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_programmerKeypad, &ProgrammerKeypad::buttonThemeChanged);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_proListDelegate, &ProListDelegate::setThemeType);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            m_proExpressionBar, &ProExpressionBar::initTheme);
    connect(m_checkBtnKeypad, &ProCheckBtnKeypad::buttonPressed, this, &ProgramModule::handleCheckBtnKeypadButtonPress);
//    connect(m_proExpressionBar, &ProExpressionBar::keyPress, this, &ProgramModule::handleEditKeyPress);
}

ProgramModule::~ProgramModule()
{

}

void ProgramModule::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void ProgramModule::mousePressEvent(QMouseEvent *event)
{
    m_byteArrowRectangle->setHidden(true);
    m_shiftArrowRectangle->setHidden(true);
    setwidgetAttribute(false);
    DWidget::mousePressEvent(event);
}

void ProgramModule::handleCheckBtnKeypadButtonPress(int key)
{
    QString path;
    if (DGuiApplicationHelper::instance()->themeType() == 2)
        path = QString(":/assets/images/%1/").arg("dark");
    else
        path = QString(":/assets/images/%1/").arg("light");
    m_checkBtnKeypad->update();
    switch (key) {
    case ProCheckBtnKeypad::Key_GeneralKeypad:
        //点击后设置Key_GeneralKeypad为点击状态，Key_BinaryKeypad为普通状态
        static_cast<IconButton *>(m_checkBtnKeypad->button(ProCheckBtnKeypad::Key_GeneralKeypad))->
        setIconUrl(path + "icon_generalkeyboard_press.svg", path + "icon_generalkeyboard_press.svg", path + "icon_generalkeyboard_press.svg", 3);
        static_cast<IconButton *>(m_checkBtnKeypad->button(ProCheckBtnKeypad::Key_BinaryKeypad))->
        setIconUrl(path + "icon_binarysystem_normal.svg", path + "icon_binarysystem_hover.svg", path + "icon_binarysystem_normal.svg", 3);
        m_stackWidget->setCurrentWidget(m_programmerKeypad);
        break;
    case ProCheckBtnKeypad::Key_BinaryKeypad:
        //点击后设置Key_BinaryKeypad为点击状态，Key_GeneralKeypad为普通状态
        static_cast<IconButton *>(m_checkBtnKeypad->button(ProCheckBtnKeypad::Key_BinaryKeypad))->
        setIconUrl(path + "icon_binarysystem_press.svg", path + "icon_binarysystem_press.svg", path + "icon_binarysystem_press.svg", 3);
        static_cast<IconButton *>(m_checkBtnKeypad->button(ProCheckBtnKeypad::Key_GeneralKeypad))->
        setIconUrl(path + "icon_generalkeyboard_normal.svg", path + "icon_generalkeyboard_hover.svg", path + "icon_generalkeyboard_normal.svg", 3);
        m_stackWidget->setCurrentWidget(m_proSystemKeypad);
        break;
    case ProCheckBtnKeypad::Key_System:
        m_byteArrowRectangle->setHidden(false);
        setwidgetAttribute(true);
        m_byteArrowRectangle->setFocus();
        break;
    case ProCheckBtnKeypad::Key_Option:
        m_shiftArrowRectangle->setHidden(false);
        setwidgetAttribute(true);
        m_shiftArrowRectangle->setFocus();
        break;
    case ProCheckBtnKeypad::Key_Mlist:
        break;
    case ProCheckBtnKeypad::Key_MS:
        break;
    default:
        break;
    }
}

/**
 * @brief 算法悬浮框点击事件
 */
void ProgramModule::shiftArrowListWidgetItemClicked(QListWidgetItem *item)
{
    QString path;
    if (DGuiApplicationHelper::instance()->themeType() == 2)
        path = QString(":/assets/images/%1/").arg("dark");
    else
        path = QString(":/assets/images/%1/").arg("light");
    m_shiftArrowListWidget->setCurrentItem(item);
    m_shiftArrowRectangle->setHidden(true);
    setwidgetAttribute(false);

    //计算方式选项按钮图标跟随选项改变
    switch (m_shiftArrowListWidget->currentRow()) {
    case 0:
        static_cast<IconButton *>(m_checkBtnKeypad->button(ProCheckBtnKeypad::Key_Option))->
        setIconUrl(path + "icon_as_normal.svg", path + "icon_as_hover.svg", path + "icon_as_normal.svg", 3);
        break;
    case 1:
        static_cast<IconButton *>(m_checkBtnKeypad->button(ProCheckBtnKeypad::Key_Option))->
        setIconUrl(path + "icon_ls_normal.svg", path + "icon_ls_hover.svg", path + "icon_ls_normal.svg", 3);
        break;
    case 2:
        static_cast<IconButton *>(m_checkBtnKeypad->button(ProCheckBtnKeypad::Key_Option))->
        setIconUrl(path + "icon_ro_normal.svg", path + "icon_ro_hover.svg", path + "icon_ro_normal.svg", 3);
        break;
    case 3:
        static_cast<IconButton *>(m_checkBtnKeypad->button(ProCheckBtnKeypad::Key_Option))->
        setIconUrl(path + "icon_rc_normal.svg", path + "icon_rc_hover.svg", path + "icon_rc_normal.svg", 3);
        break;
    }
}

/**
 * @brief 位数悬浮框点击事件
 */
void ProgramModule::byteArrowListWidgetItemClicked(QListWidgetItem *item)
{
    m_byteArrowListWidget->setCurrentItem(item);
    QString str = static_cast<ProgrammerItemWidget *>(m_byteArrowListWidget->itemWidget(m_byteArrowListWidget->currentItem()))->findChild<QLabel *>()->text();
    static_cast<TextButton *>(m_checkBtnKeypad->button(ProCheckBtnKeypad::Key_System))->setText(str);
    m_byteArrowRectangle->setHidden(true);
    setwidgetAttribute(false);

    //改变m_proSystemKeypad按钮状态
    switch (m_byteArrowListWidget->currentRow()) {
    case 0:
        m_proSystemKeypad->setSystem(64);
        break;
    case 1:
        m_proSystemKeypad->setSystem(32);
        break;
    case 2:
        m_proSystemKeypad->setSystem(16);
        break;
    case 3:
        m_proSystemKeypad->setSystem(8);
        break;
    }
}

/**
 * @brief 主题变换checkBtnKeypad切换图片
 */
void ProgramModule::checkBtnKeypadThemeChange(int type)
{
    QString path;
    if (DGuiApplicationHelper::instance()->themeType() == 2)
        path = QString(":/assets/images/%1/").arg("dark");
    else
        path = QString(":/assets/images/%1/").arg("light");
    m_checkBtnKeypad->buttonThemeChanged(type);
    //给m_checkBtnKeypad前两个按钮切图
    if (m_stackWidget->currentWidget() == m_programmerKeypad) {
        //设置Key_GeneralKeypad为点击状态
        static_cast<IconButton *>(m_checkBtnKeypad->button(ProCheckBtnKeypad::Key_GeneralKeypad))->
        setIconUrl(path + "icon_generalkeyboard_press.svg", path + "icon_generalkeyboard_press.svg", path + "icon_generalkeyboard_press.svg", 3);
    } else if (m_stackWidget->currentWidget() == m_proSystemKeypad) {
        //设置Key_BinaryKeypad为点击状态
        static_cast<IconButton *>(m_checkBtnKeypad->button(ProCheckBtnKeypad::Key_BinaryKeypad))->
        setIconUrl(path + "icon_binarysystem_press.svg", path + "icon_binarysystem_press.svg", path + "icon_binarysystem_press.svg", 3);
    }
}

void ProgramModule::initArrowRectangle()
{
    QString path;
    if (DGuiApplicationHelper::instance()->themeType() == 2)
        path = QString(":/assets/images/%1/").arg("dark");
    else
        path = QString(":/assets/images/%1/").arg("light");

    m_shiftArrowListWidget->setItemDelegate(m_shiftProgrammerArrowDelegate);
    m_shiftArrowListWidget->setFrameShape(QFrame::NoFrame); //设置边框类型，无边框
    m_shiftArrowListWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    QListWidgetItem *item1 = new QListWidgetItem();
    ProgrammerItemWidget *itemwidget1 = new ProgrammerItemWidget("算数移位", QIcon::fromTheme(path + "icon_as_hover.svg"));
    item1->setFlags(Qt::NoItemFlags);
    item1->setSizeHint(QSize(250, 34));
    m_shiftArrowListWidget->insertItem(0, item1);
    m_shiftArrowListWidget->setItemWidget(item1, itemwidget1);
    QListWidgetItem *item2 = new QListWidgetItem();
    ProgrammerItemWidget *itemwidget2 = new ProgrammerItemWidget("逻辑移位", QIcon::fromTheme(path + "icon_ls_hover.svg"));
    item2->setFlags(Qt::NoItemFlags);
    item2->setSizeHint(QSize(250, 34));
    m_shiftArrowListWidget->insertItem(1, item2);
    m_shiftArrowListWidget->setItemWidget(item2, itemwidget2);
    QListWidgetItem *item3 = new QListWidgetItem();
    ProgrammerItemWidget *itemwidget3 = new ProgrammerItemWidget("旋转循环移位", QIcon::fromTheme(path + "icon_ro_hover.svg"));
    item3->setFlags(Qt::NoItemFlags);
    item3->setSizeHint(QSize(250, 34));
    m_shiftArrowListWidget->insertItem(2, item3);
    m_shiftArrowListWidget->setItemWidget(item3, itemwidget3);
    QListWidgetItem *item4 = new QListWidgetItem();
    ProgrammerItemWidget *itemwidget4 = new ProgrammerItemWidget("循环移位旋转", QIcon::fromTheme(path + "icon_rc_hover.svg"));
    item4->setFlags(Qt::NoItemFlags);
    item4->setSizeHint(QSize(250, 34));
    m_shiftArrowListWidget->insertItem(3, item4);
    m_shiftArrowListWidget->setItemWidget(item4, itemwidget4);

    //默认计算方式为算术移位
    m_shiftArrowListWidget->setCurrentItem(m_shiftArrowListWidget->item(0));
    static_cast<ProgrammerItemWidget *>(m_shiftArrowListWidget->itemWidget(m_shiftArrowListWidget->currentItem()))
    ->findChild<DIconButton *>("markBtn")->setHidden(false);

    m_shiftArrowListWidget->setFixedSize(QSize(250, 136));
//    m_shiftArrowRectangle->setWindowFlag(Qt::Popup);
    m_shiftArrowRectangle->setRadius(15);
    m_shiftArrowRectangle->setArrowWidth(48);
    m_shiftArrowRectangle->setArrowHeight(21);
    m_shiftArrowRectangle->setContent(m_shiftArrowListWidget);
    m_shiftArrowRectangle->move(this->rect().x() + 288, this->rect().y() + 238);//在module的138，238位置，x多出150原因未找出
    m_shiftArrowListWidget->move(m_shiftArrowRectangle->rect().x(),
                                 m_shiftArrowRectangle->rect().y() + m_shiftArrowRectangle->arrowHeight() + 11);

    m_shiftArrowRectangle->setWidth(250);
    m_shiftArrowRectangle->setShadowXOffset(0);
    m_shiftArrowRectangle->setShadowYOffset(0);
    m_shiftArrowRectangle->setShadowBlurRadius(0);
    m_shiftArrowRectangle->setHeight(181);
    m_shiftArrowRectangle->setHidden(true);

    connect(m_shiftArrowListWidget, &QListWidget::itemClicked, this, &ProgramModule::shiftArrowListWidgetItemClicked);
    connect(m_shiftArrowListWidget, &QListWidget::currentItemChanged, [ = ](QListWidgetItem * current, QListWidgetItem * previous) {
        static_cast<ProgrammerItemWidget *>(m_shiftArrowListWidget->itemWidget(current))->findChild<DIconButton *>("markBtn")->setHidden(false);
        if (m_shiftArrowListWidget->row(previous) != -1) {
            static_cast<ProgrammerItemWidget *>(m_shiftArrowListWidget->itemWidget(previous))->findChild<DIconButton *>("markBtn")->setHidden(true);
        }
    });

    m_byteArrowListWidget->setItemDelegate(m_shiftProgrammerArrowDelegate);
    m_byteArrowListWidget->setFrameShape(QFrame::NoFrame); //设置边框类型，无边框
    m_byteArrowListWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    QListWidgetItem *item5 = new QListWidgetItem();
    ProgrammerItemWidget *itemwidget5 = new ProgrammerItemWidget("QWORD");
    item5->setFlags(Qt::NoItemFlags);
    item5->setSizeHint(QSize(250, 34));
    m_byteArrowListWidget->insertItem(0, item5);
    m_byteArrowListWidget->setItemWidget(item5, itemwidget5);
    QListWidgetItem *item6 = new QListWidgetItem();
    ProgrammerItemWidget *itemwidget6 = new ProgrammerItemWidget("DWORD");
    item6->setFlags(Qt::NoItemFlags);
    item6->setSizeHint(QSize(250, 34));
    m_byteArrowListWidget->insertItem(1, item6);
    m_byteArrowListWidget->setItemWidget(item6, itemwidget6);
    QListWidgetItem *item7 = new QListWidgetItem();
    ProgrammerItemWidget *itemwidget7 = new ProgrammerItemWidget("WORD");
    item7->setFlags(Qt::NoItemFlags);
    item7->setSizeHint(QSize(250, 34));
    m_byteArrowListWidget->insertItem(2, item7);
    m_byteArrowListWidget->setItemWidget(item7, itemwidget7);
    QListWidgetItem *item8 = new QListWidgetItem();
    ProgrammerItemWidget *itemwidget8 = new ProgrammerItemWidget("BYTE");
    item8->setFlags(Qt::NoItemFlags);
    item8->setSizeHint(QSize(250, 34));
    m_byteArrowListWidget->insertItem(3, item8);
    m_byteArrowListWidget->setItemWidget(item8, itemwidget8);

    //默认位数64位
    m_byteArrowListWidget->setCurrentItem(m_byteArrowListWidget->item(0));
    static_cast<ProgrammerItemWidget *>(m_byteArrowListWidget->itemWidget(m_byteArrowListWidget->currentItem()))
    ->findChild<DIconButton *>("markBtn")->setHidden(false);

    m_byteArrowListWidget->setFixedSize(QSize(250, 136));
    m_byteArrowRectangle->setRadius(15);
    m_byteArrowRectangle->setArrowWidth(48);
    m_byteArrowRectangle->setArrowHeight(21);
    m_byteArrowRectangle->setContent(m_byteArrowListWidget);
    m_byteArrowRectangle->move(this->rect().x() + 215, this->rect().y() + 238);//在module的138，238位置，x多出150原因未找出
    m_byteArrowListWidget->move(m_byteArrowRectangle->rect().x(),
                                m_byteArrowRectangle->rect().y() + m_byteArrowRectangle->arrowHeight() + 11);

    m_byteArrowRectangle->setWidth(250);
    m_byteArrowRectangle->setShadowXOffset(0);
    m_byteArrowRectangle->setShadowYOffset(0);
    m_byteArrowRectangle->setShadowBlurRadius(0);
    m_byteArrowRectangle->setHeight(181);
    m_byteArrowRectangle->setHidden(true);

    connect(m_byteArrowListWidget, &QListWidget::itemClicked, this, &ProgramModule::byteArrowListWidgetItemClicked);
    connect(m_byteArrowListWidget, &QListWidget::currentItemChanged, [ = ](QListWidgetItem * current, QListWidgetItem * previous) {
        static_cast<ProgrammerItemWidget *>(m_byteArrowListWidget->itemWidget(current))->findChild<DIconButton *>("markBtn")->setHidden(false);
        if (m_byteArrowListWidget->row(previous) != -1) {
            static_cast<ProgrammerItemWidget *>(m_byteArrowListWidget->itemWidget(previous))->findChild<DIconButton *>("markBtn")->setHidden(true);
        }
    });
    connect(m_byteArrowRectangle, &ArrowRectangle::hidearrowrectangle, this, [ = ]() {
        m_byteArrowRectangle->setHidden(true);
        setwidgetAttribute(false);
    });
    connect(m_shiftArrowRectangle, &ArrowRectangle::hidearrowrectangle, this, [ = ]() {
        m_shiftArrowRectangle->setHidden(true);
        setwidgetAttribute(false);
    });
}

void ProgramModule::handleEditKeyPress(QKeyEvent *)
{

}

/**
 * @brief ProgramModule::setwidgetAttribute
 * 设置鼠标能否点击module中的其他控件，用于点出rectangle和内存列表
 * @param b
 * true:无法点击 false：可以点击
 */
void ProgramModule::setwidgetAttribute(bool b)
{
    m_proExpressionBar->setAttribute(Qt::WA_TransparentForMouseEvents, b);
    m_programmerKeypad->setAttribute(Qt::WA_TransparentForMouseEvents, b);
    m_proSystemKeypad->setAttribute(Qt::WA_TransparentForMouseEvents, b);
    m_checkBtnKeypad->setAttribute(Qt::WA_TransparentForMouseEvents, b);
    m_proListView->setAttribute(Qt::WA_TransparentForMouseEvents, b);
}