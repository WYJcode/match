#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include <time.h>
#include<stdlib.h>
#include <stack>
#include <process.h>
#include<iostream>
using namespace std;
//加密码，保存数据进入文档时向前移动28位
#define encryptionKey 28

int choice = 0, choice_1 = 0;//功能选择标记
char file_name_now[100];  //全局保存加载文件路径
FILE *fp = NULL;//文件指针
char txt[5] = ".txt";//文件后缀
char input[30];//输入文件名
char re[30];//文件全名
int colour[16] = { 240, 15, 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224 };
stack<int>xy;//保存每行x的位置
int wbscmh = 0;//文本输出末行

struct wenzi
{
	char f[2];//f[0] = 0 f[1] = 'a'  f[0] = 高位内码 f[1] = 地位内码
	int color;//16：15、16、32、48、64、80、96、112、128、144、160、176、192、208、224、240号色字体
	int size;//10字体大小
	int font;//16字体类型
	struct wenzi *next;//下一个节点
	struct wenzi *previous;//上一个节点
};

struct wenzi *head, *tail, *m, *clipboard;//头，尾，移动, 剪贴板

void gotoxy(int x, int y);//字体位置

void color(int c);//字体颜色

void windowSettings();//窗口设置

void toolbar();//工具栏

void textBox();//文本框

void functionDisplay();//功能显示

void file();//文件功能显示

void edit();//编辑功能显示

void format();//格式工呢显示

void see();//查看功能显示

void about();//关于功能显示

void textOutput();//文本输出

void newNotepad();//新建记事本

void loadNotepad(); //加载记事本

void readTextData();//读取文本数据

void preservationFile();//保存文件

void encryption();//加密

void decrypt();//解密

void backgroundColor();//背景颜色

void shear();//剪切

void copy();//复制

void paste();//粘贴

void del();//删除

int main()
{
	//test();
	char lj[] = "mkdir c:\\一鸣记事本";
	system(lj);
	windowSettings();//输出窗口设置
	toolbar();//输出工具栏
	textBox();//输出文本框
	return 0;
}

void gotoxy(int x, int y)//字体位置
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void color(int c)//字体颜色
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void windowSettings()//窗口设置
{
	system("mode con cols=100 lines=40");//修改窗口cols行、lines列大小

	SetConsoleTitle(L"文本编辑器");//设置窗口标题

	system("color F0");//设置窗口颜色
}

void toolbar()//工具栏
{
	gotoxy(0, 0);//在第0行第0列位置输出
	//如果移动到相应功能就输出黑色背景白色字否则就输出字体颜色为白色背景黑色字
	if (choice == 1){
		color(112);
	}
	printf("  文  件  ");
	color(240);
	if (choice == 2){
		color(112);
	}
	printf("  编  辑  ");
	color(240);
	if (choice == 3){
		color(112);
	}
	printf(" 格  式  ");
	color(240);
	if (choice == 4){
		color(112);
	}
	printf("  查  看  ");
	color(240);
	if (choice == 5){
		color(112);
	}
	printf("  关  于  \n");
	color(240);

	textOutput();//文本输出
	//choice==0不在功能选择
	if (choice != 0){
		char ch = getch();//接受一个输入字符不需要输入回车键
		switch (ch){
		case 75://输入左键
			//如果现在是最左边就改为最右边否则就往左移一位
			if (choice == 1){
				choice = 5;
			}
			else{
				choice--;
			}
			break;
		case 77://输入右键
			//如果现在是最右边就改为最左边否则就往右移一位
			choice %= 5;
			choice++;
			break;
		case 27://输入Esc
			//退出功能选择
			choice = 0;
			break;
		case 13://回车键
			//选择具体功能
			functionDisplay();//功能显示
			break;
		}
	}
}

void textBox()//文本框
{
	int x = 0, y = 1;//x，y保存当前光标位置
	head = (struct wenzi *)malloc(sizeof(struct wenzi));//创建数据链表头结点
	struct wenzi *p, *q;//当前链表，前一个链表
	p = head;
	q = NULL;
	p->next = NULL;
	//先创建6个密码数据指针
	for (int i = 0; i < 6; i++){
		p->f[0] = '\0';
		p->f[1] = '\0';
		p->color = 0;
		p->font = 0;
		p->size = 0;
		p->previous = q;
		q = p;
		p = (struct wenzi *)malloc(sizeof(struct wenzi));
		p->next = NULL;
		q->next = p;
	}
	p = q;
	p->next = NULL;
	q = p->previous;
	tail = p;
	char ch[2];//保存输入数据
	while (true){
		ch[0] = getch();//接受一个输入字符
		if (ch[0] == 27){//输入Esc键进入功能选择
			choice = 1;
			while (choice != 0){
				toolbar();//不断接受功能的选择输出
			}
			toolbar();//退出功能选择再输出一次无选择的功能栏
			p = tail;//p等于尾指针
			q = p->previous;
			//获取文本输出末行的位置
			if (wbscmh != 0){
				x = wbscmh;
				wbscmh = 0;
			}
		}
		/*
		接受左右键，按左键指针就往前移一位，按右键指针就往后移一位
		*/
		else if (ch[0] == -32){//方向键等扩展键
			ch[1] = getch();
		}
		/*
		在指针位置添加一个换行键
		*/
		else if (ch[0] == 13){//回车键
			p = (struct wenzi *)malloc(sizeof(struct wenzi));
			p->next = NULL;
			//把p链接上数据指针末尾
			q = q->next;//q等于下一个节点
			q->next = p;//q的下一个节点等于p
			p->f[0] = '\n';
			p->f[1] = '\0';
			p->color = 0;
			p->font = 0;
			p->size = 0;
			p->previous = q;
			printf("\n");
			xy.push(x);//存储y行的x列数
			y++;
			x = 0;
			tail = p;//尾指针等于p
		}
		/*
		删除指针所指位置的数据
		*/
		else if (ch[0] == 8){//删除键
			if (!(x == 0 && y == 1)){//只要不是在开头就往回退一个
				free(p);//释放p指针内存空间
				//p指针往回退一个
				p = q;
				p->next = NULL;
				q = q->previous;
				tail = p;//更新尾指针
				//如果是一行的开头就往回退一行否则就往回退一个
				if (x == 0){
					y--;
					x = xy.top();//取出y行的x列数
					xy.pop();//删除y行的x列数
					gotoxy(x, y);//定位到x列,y行位置
				}
				else{
					x -= 2;
					gotoxy(x, y);//光标往回退一个
					printf("  ");
					gotoxy(x, y);
				}
			}
			
		}
		/*
		指针保存英文数据
		*/
		else if (ch[0] <= 126 && ch[0] >= 32){//输入英文、数字、符号
			ch[1] = '\0';//填充ch[1]位置
			p = (struct wenzi *)malloc(sizeof(struct wenzi));
			p->next = NULL;
			//把p链接上数据指针末尾
			q = q->next;
			q->next = p;
			p->previous = q;
			p->f[0] = ch[0];
			p->f[1] = ch[1];
			p->color = 0;
			p->font = 0;
			p->size = 0;
			printf("%c", ch[0]);
			printf("%c", ch[1]);
			x+=2;
			tail = p;//更新尾指针
		}
		/*
		指针保存中文数据
		*/
		else{//中文
			ch[1] = getch();//接受低位内码
			p = (struct wenzi *)malloc(sizeof(struct wenzi));
			p->next = NULL;
			q = q->next;
			q->next = p;
			p->previous = q;
			p->f[0] = ch[0];
			p->f[1] = ch[1];
			p->color = 0;
			p->font = 0;
			p->size = 0;
			printf("%c", ch[0]);
			printf("%c", ch[1]);
			x += 2;
			tail = p;//更新尾指针
		}
	}
}

void functionDisplay()//功能显示
{
	system("cls");//清屏
	printf("  文  件  ");
	printf("  编  辑  ");
	printf(" 格  式  ");
	printf("  查  看  ");
	printf("  关  于  \n");
	switch (choice){//跳到相应的功能函数
	case 1:
		choice_1 = 1;
		while (choice_1 != 0){
			file();//文件功能显示
		}
		system("cls");
		break;
	case 2:
		choice_1 = 1;
		while (choice_1 != 0){
			edit();//编辑功能显示
		}
		system("cls");
		break;
	case 3:
		choice_1 = 1;
		while (choice_1 != 0){
			format();//格式工呢显示
		}
		system("cls");
		break;
	case 4:
		see();//查看功能显示
		break;
	case 5:
		about();//关于功能显示
		break;
	}
}

void file()//文件功能显示
{
	textOutput();//输出文本
	gotoxy(0, 1);
	if (choice_1 == 1){
		color(112);
	}
	printf("  新  建  ");
	color(240);
	gotoxy(0, 2);
	if (choice_1 == 2){
		color(112);
	}
	printf("  打  开  ");
	color(240);
	gotoxy(0, 3);
	if (choice_1 == 3){
		color(112);
	}
	printf("  保  存  ");
	color(240);
	gotoxy(0, 4);
	if (choice_1 == 4){
		color(112);
	}
	printf("  加  密  ");
	color(240);
	gotoxy(0, 5);
	if (choice_1 == 5){
		color(112);
	}
	printf("  退  出  ");
	color(240);
	if (choice_1 != 0){
		char ch = getch();
		switch (ch){
		case 72://输入上键
			if (choice_1 == 1){
				choice_1 = 5;
			}
			else{
				choice_1--;
			}
			break;
		case 80://输入下键
			choice_1 %= 5;
			choice_1++;
			break;
		case 27://输入Esc
			choice_1 = 0;
			break;
		case 13://输入回车键
			choice = 0;
			switch (choice_1){
			case 1:
				newNotepad();//新建文件
				break;
			case 2:
				loadNotepad();//打开文件
				break;
			case 3:
				preservationFile();//保存文件
				break;
			case 4:
				encryption();//加密文件
				break;
			case 5:
				exit(0);//退出程序
			}
			choice_1 = 0;
			break;
		}
	}
}

void edit()//编辑功能显示
{
	textOutput();
	gotoxy(10, 1);
	if (choice_1 == 1){
		color(112);
	}
	printf("  剪  切  ");
	color(240);
	gotoxy(10, 2);
	if (choice_1 == 2){
		color(112);
	}
	printf("  复  制  ");
	color(240);
	gotoxy(10, 3);
	if (choice_1 == 3){
		color(112);
	}
	printf("  粘  贴  ");
	color(240);
	gotoxy(10, 4);
	if (choice_1 == 4){
		color(112);
	}
	printf("  删  除  ");
	color(240);
	if (choice != 0){
		char ch = getch();
		switch (ch){
		case 72://上键
			if (choice_1 == 1){
				choice_1 = 4;
			}
			else{
				choice_1--;
			}
			break;
		case 80://下键
			choice_1 %= 4;
			choice_1++;
			break;
		case 27://Esc
			choice_1 = 0;
			break;
		case 13://回车键
			choice = 0;
			switch (choice_1){
			case 1:
				shear();//剪切
				break;
			case 2:
				copy();//复制
				break;
			case 3:
				paste();//粘贴
				break;
			case 4:
				del();//删除
				break;
			}
			choice_1 = 0;
			break;
		}
	}
}

void format()//格式功能显示
{
	textOutput();
	gotoxy(19, 1);
	if (choice_1 == 1){
		color(112);
	}
	printf("  颜  色  ");
	color(240);
	gotoxy(19, 2);
	if (choice_1 == 2){
		color(112);
	}
	printf("  字  体  ");
	color(240);
	if (choice_1 != 0){
		char ch = getch();
		switch (ch){
		case 72://上键
			if (choice_1 == 1){
				choice_1 = 2;
			}
			else{
				choice_1--;
			}
			break;
		case 80://下键
			choice_1 %= 2;
			choice_1++;
			break;
		case 27://Esc
			choice_1 = 0;
			break;
		case 13://回车键
			choice_1 = 1;
			if (choice_1 == 1){//改变背景颜色
				backgroundColor();
			}
			else{

			}
			choice_1 = 0;
			choice = 0;
			break;
		}
	}
}

void see()//查看功能显示
{
	textOutput();//文本输出
	choice = 0;
	choice_1 = 0;
	gotoxy(0, 15);
	color(colour[15]);

	time_t now;
	struct tm *tm_now;
	time(&now);
	tm_now = localtime(&now);

	int z = 0, dy = 0, xy = 0, f = 0, s = 0;
	struct wenzi *p;
	p = head;
	for (int i = 0; i < 6; i++){
		p = p->next;
	}
	while (p != NULL){
		if (p->f[1] != '\0'){
			z++;
		}
		else if (p->f[0] >= 'A' && p->f[0] <= 'Z'){
			dy++;
		}
		else if (p->f[0] >= 'a' && p->f[0] <= 'z'){
			xy++;
		}
		else if (p->f[0] >= '0'&& p->f[0] <= '9'){
			s++;
		}
		else{
			f++;
		}
		p = p->next;
	}
	printf("\t\t\t\t\t中文共有：%d\n", z);
	printf("\t\t\t\t\t大写英文共有：%d\n", dy);
	printf("\t\t\t\t\t小写英文共有：%d\n", xy);
	printf("\t\t\t\t\t数字共有：%d\n", s);
	printf("\t\t\t\t\t字符共有：%d\n", f);
	printf("\t\t\t\t\t当前时间: %d-%d-%d %d:%d:%d\n",tm_now->tm_year + 1900, tm_now->tm_mon + 1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
	color(colour[0]);
	char ch = getch();//定格画面
	system("cls");
}

void about()//关于功能显示
{
	textOutput();//文本输出
	choice = 0;
	choice_1 = 0;
	gotoxy(0, 15);
	color(colour[15]);
	gotoxy(0, 8);
	color(colour[15]);
	//添加关于操作指导输出
	cout<<"\t\t\t\t操作指导:"<<endl;
	cout<<"\t\t\t\t1.通过使用Esc 键进入或退出功能选择项"<<endl;
	cout<<"\t\t\t\t2.在菜单栏中可以通过上下左右键来对相"<<endl;cout<<"\t\t\t\t应功能进行选择，并通过回车键进行实施"<<endl;
	cout<<"\t\t\t\t3.选择需要的相应子菜单功能需要通过回"<<endl;cout<<"\t\t\t\t车键开始实施功能，再次回车可退出功能"<<endl;
	cout<<"\t\t\t\t4.选择文本框的字段时是从头开始往右选"<<endl;cout<<"\t\t\t\t择的回车键开始实施功能再次回车可退出"<<endl;
	cout<<"\t\t\t\t5.编辑除了粘贴其他功能都需要'文本框'"<<endl;cout<<"\t\t\t\t(内容区)已存在数据内容，才可编辑操作"<<endl<<endl;
	
	color(colour[11]);
	
	cout<<"\t\t\t\t6.'文件'，选择'文件'主菜单，再选择相"<<endl;cout<<"\t\t\t\t应的子菜单通过回车键确认便可实施功能"<<endl;
	cout<<"\t\t\t\t7.'编辑'，选择'编辑'主菜单，再选择相"<<endl;cout<<"\t\t\t\t应的子菜单功能，便可开始对文本进行编"<<endl;
	cout<<"\t\t\t\t辑操作( 即首先通过回车选择开始编辑的"<<endl;cout<<"\t\t\t\t字符,右键选择编辑区域，回车完成退出)"<<endl; 
	cout<<"\t\t\t\t子功能以及选择字段目前都尚未支持左键"<<endl;
	cout<<"\t\t\t\t8.'格式'，选择'格式'主菜单，再选择相"<<endl;cout<<"\t\t\t\t应的子菜单回车键确认便可实施功能( 左"<<endl;
	cout<<"\t\t\t\t右回车键来选择颜色再对文本进行选择 )"<<endl; 
	cout<<"\t\t\t\t9.'查看'，选择'查看'主菜单，便可对当"<<endl;cout<<"\t\t\t\t前输入数据以及时间进行相关的统计显示"<<endl<<endl;
	color(colour[8]);
	cout<<"\t\t\t\t一鸣文本编辑器"<<endl; 
	printf("\t\t\t\tVersion 1.0\n\t\t\t\t文育金、王庆林、李伟铭\n\t\t\t\t共同完成");

	color(colour[0]);
	char ch = getch();//定格画面
	system("cls");
}

void textOutput()//文本输出
{
	color(241);
	gotoxy(50, 0);
	printf("按Esc键进入或退出功能选择项");
	gotoxy(0, 1);
	color(240);
	if (head != NULL){//head头指针不能为空
		gotoxy(0, 1);
		int num = 0;//一行的字数
		struct wenzi *p;
		p = head;
		//跳过6个密码指针
		for (int i = 0; i < 6; i++){
			p = p->next;
		}
		while (p != NULL){//p指针唯空时，输出结束
			color(colour[p->color]);
			printf("%c", p->f[0]);
			printf("%c", p->f[1]);
			if (p->f[0] == 13){//p->f[0]唯换行键
				xy.push(num * 2);
				num = 0;//更新一行的字数
			}
			else{
				num++;//字数加一
			}
			if (num == 80){//每行八十个字就换行
				printf("\n");
				xy.push(num * 2);
				num = 0;
			}
			if (p->next == NULL){//最后一个指针
				wbscmh = num * 2;//保存文本输出末行
				tail = p;//保存末尾指针
			}
			p = p->next;
		}
	}
	return;
}

void newNotepad()  //新建记事本
{
	int flag;
	for (flag = 0; flag == 0;)
	{
		system("cls");
		printf("\t\t\t****************************\n");
		printf("\t\t\t        新 记 事 本 \n");
		printf("\t\t\t****************************\n\n");
		strcpy(re, "c:\\一鸣记事本\\");
		printf("\t\t\t     *请输入记事本名称:\n\t\t\t    ");
		scanf("%s", input);
		strcat(re, input);//input字符串添加到re字符串后面,re保存文件命
		strcat(re, txt);//txt字符串添加到re字符串后面，re添加后缀名
		if ((fp = fopen(re, "rb")) != NULL)
		{
			printf("\n\n\n\t\t - - - - - - - - - - - - - - - - - - - - - - -\n\t\t   新记事本和原有记事本名称重复，请重新命名！\n\t\t\t    ");
			system("pause");
			flag = 0;
		}
		else
		{
			system("cls");
			printf("\t\t\t****************************\n");
			printf("\t\t\t        新 记 事 本 \n");
			printf("\t\t\t****************************\n\n");
			fp = fopen(re, "ab+");
			printf("\t\t\t已创建记事本：%s\n\n\t\t\t     ", re);
			flag = 1;
			system("pause");
		}
	}
	strcpy(file_name_now, re);//re字符串复制给file_name_now字符串
	return;
}

void loadNotepad() //加载记事本
{
	system("cls");
	printf("\t\t\t****************************\n");
	printf("\t\t\t       加 载 记 事 本 \n");
	printf("\t\t\t****************************\n\n");
	strcpy(re, "C:\\一鸣记事本\\");
	printf("\t\t\t      *已保存的记事本：\n");
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
	system("dir /b C:\\一鸣记事本");
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
	printf("\t\t\t\n*请输入记事本名称：");
	scanf("%s", input);
	strcat(re, input);//input字符串添加到re字符串后面,re保存文件命
	strcat(re, txt);//txt字符串添加到re字符串后面，re添加后缀名
	if ((fp = fopen(re, "rb")) == NULL)//打开re文件读
	{
		printf("\n\n\n- - - - - - - - - - - -\n没有该记事本。\n");
		fp = fopen(re, "ab+");//创建re文件，ab+ 读写打开一个二进制文件如果没有就创建一个，允许从头开始读或在文件末追加数据。
		printf("\t\t\t已创建记事本：%s\n\n\t\t\t     ", re);
		strcpy(file_name_now, re);
		system("pause");//系统停留
	}
	else
	{
		strcpy(file_name_now, re);//re字符串复制给file_name_now字符串
		readTextData();//读取文件数据
		decrypt();//解密文件
	}
}

void readTextData()//读取文本数据
{
	char c;
	struct wenzi *p, *q;
	//清楚原本数据
	while (head != NULL){
		p = head->next;
		free(head);
		head = p;
	}
	//重新创建head结点
	head = (struct wenzi *)malloc(sizeof(struct wenzi));
	p = head;
	q = NULL;
	p->next = NULL;
	//读取文件内容
	while ((c = fgetc(fp)) != EOF){
		//提取加密码，因为在存入时所有数据都加了一次加密码，所以提取时都要减去加密码
		p->f[0] = c - encryptionKey;
		c = fgetc(fp);
		p->f[1] = c - encryptionKey;
		c = fgetc(fp);
		p->color = c - encryptionKey;
		c = fgetc(fp);
		p->size = c - encryptionKey;
		c = fgetc(fp);
		p->font = c - encryptionKey;
		q = p;
		p = (struct wenzi *)malloc(sizeof(struct wenzi));
		p->next = NULL;
		p->previous = q;
		q->next = p;
	}
	//清楚最后一个空指针
	free(p);
	p = q;
	//如果没有读到数据就生成一个带有6个结点的空数据指针密码
	if (p == NULL){
		head = (struct wenzi *)malloc(sizeof(struct wenzi));
		p = head;
		p->next = NULL;
		for (int i = 0; i < 6; i++){
			p->f[0] = '\0';
			p->f[1] = '\0';
			p->color = 0;
			p->font = 0;
			p->size = 0;
			p->previous = q;
			q = p;
			p = (struct wenzi *)malloc(sizeof(struct wenzi));
			p->next = NULL;
			q->next = p;
		}
		free(p);
		p = q;
		p->next = NULL;
		tail = p;
		m = p;
	}
	else{
		p->next = NULL;
		tail = p;//更新尾指针
		m = p;
	}
}

void preservationFile()//保存文件
{
	//如果没有打开文件就创建一个文件
	if (fp == NULL){
		newNotepad();
	}
	char del[150] = "del ";
	fclose(fp);//关闭打开的文件
	strcat(del, re);
	system(del);//删除文件
	fp = fopen(re, "ab+");//创建读写文件
	rewind(fp);//定位光标到头
	struct wenzi *p;
	char c;
	p = head;
	while (p != NULL)       //把链表中的文本数据存入文件
	{
		c = p->f[0] + encryptionKey;
		fputc(c, fp);
		c = p->f[1] + encryptionKey;
		fputc(c, fp);
		c = p->color + encryptionKey;
		fputc(c, fp);
		c = p->size + encryptionKey;
		fputc(c, fp);
		c = p->font + encryptionKey;
		fputc(c, fp);
		p = p->next;
	}
	printf("\n- - - - - - - -\n保存成功！\n");
	system("pause");
}

void encryption()//加密
{
	struct wenzi *p;
	p = head;
	system("cls");
	printf("\t\t\t****************************\n");
	printf("\t\t\t\t加密密码：");
	int num = 0;//输入密码数
	int x = 42;
	gotoxy(x, 1);
	while(true){
		char ch = getch();
		if (ch == 27){//Esc键
			
		}
		else if (ch == -32){//方向键等扩展键
			ch = getch();
		}
		else if (ch == 13){//回车键
			if(num < 6){
				gotoxy(32, 2);
				printf("密码是少于6位");
				gotoxy(x, 1);
			}
			else{
				break;
			}
		}
		else if (ch == 8){//删除键
			if (num > 0){
				p = p->previous;
				x--;
				gotoxy(x, 1);
				printf(" ");
				gotoxy(x, 1);
				num--;
			}
			
		}
		else if (ch <= 126 && ch >= 32 && num < 6){//英文字符
			p->f[0] = ch;
			p->f[1] = '\0';
			printf("*");
			x++;
			num++;
			p = p->next;
		}
		else{//中文
			if (num < 6){
				p->f[0] = ch;
				ch = getch();
				p->f[1] = ch;
				printf("*");
				x++;
				num++;
				p = p->next;
			}
		}
	}
}

void decrypt()//解密
{
	struct wenzi *p;
	p = head;
	char ch, mm[6][2];
	if (p->f[0] != '\0'){
		system("cls");
		printf("\t\t\t****************************\n");
		printf("\t\t\t\t输入密码：");
		int num = 0;
		int x = 42;
		gotoxy(x, 1);
		while (true){
			char ch = getch();
			if (ch == 27){//Esc键

			}
			else if (ch == -32){//方向键等扩展键
				ch = getch();
			}
			else if (ch == 13){//回车键
				if (num < 6){
					gotoxy(32, 2);
					printf("密码是少于6位");
					gotoxy(x, 1);
				}
				else{
					bool pd = true;
					for (int i = 0; i < 6; i++){
						if (p->f[0] != mm[i][0] || p->f[1] != mm[i][1]){
							pd = false;
							break;
						}
						p = p->next;
					}
					if (pd){
						break;
					}
					else{
						gotoxy(32, 2);
						printf("密码不正确");
						gotoxy(x, 1);
						p = head;
					}
				}
			}
			else if (ch == 8){//删除键
				if (num > 0){
					x--;
					gotoxy(x, 1);
					printf(" ");
					gotoxy(x, 1);
					num--;
				}

			}
			else if (ch >= 32 && ch <= 126 && num < 6){//英文
				mm[num][0] = ch;
				mm[num][1] = '\0';
				printf("*");
				x++;
				num++;
			}
			else{//中文
				if (num < 6){
					mm[num][0] = ch;
					ch = getch();
					mm[num][1] = ch;
					printf("*");
					x++;
					num++;
				}
			}
		}

	}
}

void backgroundColor()//改变背景颜色
{
	int x, y;
	char ch;
	x = 28;
	y = 1;
	gotoxy(x, y);
	for (int i = 0; i < 8; i++){
		color(colour[i]);
		printf("□");
	}
	x = 28;
	y = 2;
	gotoxy(x, y);
	for (int i = 0; i < 8; i++){
		color(colour[i+8]);
		printf("□");
	}
	color(colour[0]);
	x = 28;
	y = 1;
	gotoxy(x, y);
	while (ch = getch(), ch != 27){

		//选择改变部分字体背景颜色
		if (ch == 13){
			//清屏重新输出界面
			system("cls");
			choice = 0;
			toolbar();
			textOutput();
			//重新定位光标到文本框开头
			x = 0;
			y = 1;
			gotoxy(x, y);
			//存储文本指针头，过掉前六个密码指针
			struct wenzi *p, *w, *q;
			p = head;
			int direction = 0;//方向1为右，-1为左
			int start = 0;//0还没开始，1开始，2结束
			stack<int>xy;//存储每行的字数
			int num = 0;//行的字数
			for (int i = 0; i < 6; i++){
				p = p->next;
			}
			w = p;
			if (p != NULL){
				while (ch = getch(), ch != 27){
					if (ch == 13){
						start++;
						if (start == 2){
							break;
						}
						else if (start == 1){
							p->color = choice_1 - 1;
							color(colour[p->color]);
							printf("%c", p->f[0]);
							printf("%c", p->f[1]);
							gotoxy(x, y);
						}
					}
					else{
						ch = getch();
						switch (ch){
						//case 75://输入左键
						//	if (start == 1){
						//		p = p->previous;
						//		if (direction != 1)
						//	}
						//	if (p != w){
						//		if (x == 0){
						//			num = xy.top();
						//			x = num;
						//		}
						//	}
						//	break;
						case 77://输入右键
							if (p->next != NULL){
								p = p->next;
								num++;
								x += 2;
								gotoxy(x, y);
								if (start == 1){
									if (p->f[0] == '\n'){
										num = 0;
										x = 0;
										y++;
										gotoxy(x, y);
									}
									else{
										p->color = choice_1 - 1;
										printf("%c", p->f[0]);
										printf("%c", p->f[1]);
										gotoxy(x, y);
									}
								}
								if (num == 80){
									num = 0;
									x = 0;
									y++;
									gotoxy(x, y);
								}
							}
							break;
						}
					}
				}
			}
			color(colour[0]);
			break;
		}


		//选择那个背景颜色
		else{
			ch = getch();
			switch (ch){
				//case 72://输入上键
				//	break;
				//case 80://输入下键
				//	break;
			case 75://输入左键
				if (choice_1 == 1){
					choice_1 = 16;
					//光标定位到最后一个
					x = 28 + 14;
					y++;
				}
				else{
					choice_1--;
					//如果光标在第二排第一个就定位到第一排最后一个
					if (x == 28){
						x = 28 + 14;
						y--;
					}
					else{
						x -= 2;
					}
				}
				gotoxy(x, y);
				break;
			case 77://输入右键
				choice_1 %= 16;
				choice_1++;
				//光标在第一排最后一个时，把鼠标放在第二排第一个
				if (x == 28 + 14 && y == 1){
					x = 28;
					y++;
				}
				//光标在第二排最后一个时，把光标放在第一排第一个
				else if (x == 28 + 14 && y == 2){
					x = 28;
					y--;
				}
				else{
					x += 2;
				}
				gotoxy(x, y);
				break;
			}
		}
	}
}

void shear()//剪切
{
	int x , y;
	char ch;
	//选择剪切部分字
	//清屏重新输出界面
	system("cls");
	choice = 0;
	toolbar();
	textOutput();
	//重新定位光标到文本框开头
	x = 0;
	y = 1;
	gotoxy(x, y);
	//存储文本指针头，过掉前六个密码指针
	struct wenzi *p, *cp = clipboard;//文本指针，剪贴板指针
	p = head;
	int start = 0;//0还没开始，1开始，2结束
	int num = 0;//行的字数
	for (int i = 0; i < 6; i++){
		p = p->next;
	}
	if (p != NULL){
		while (true){
			ch = getch();
			if (ch == 13){
				start++;
				if (start == 2){
					//结束，把链表拿出
					clipboard->previous->next = p->next;
					p->next = NULL;
					break;
				}
				else if (start == 1){
					//清楚剪贴板内容
					while (clipboard != NULL){
						cp = clipboard->next;
						free(clipboard);
						clipboard = cp;
					}
					//记录从开始
					clipboard = p;
					color(colour[9]);
					printf("%c", p->f[0]);
					printf("%c", p->f[1]);
					gotoxy(x, y);
				}
			}
			else{
				ch = getch();
				switch (ch){
					//case 75://输入左键
					//	break;
				case 77://输入右键
					if (p->next != NULL){
						p = p->next;
						num++;
						x += 2;
						gotoxy(x, y);
						if (start == 1){
							if (p->f[0] == '\n'){
								num = 0;
								x = 0;
								y++;
								gotoxy(x, y);
							}
							else{
								printf("%c", p->f[0]);
								printf("%c", p->f[1]);
								gotoxy(x, y);
							}
						}
						if (num == 80){
							num = 0;
							x = 0;
							y++;
							gotoxy(x, y);
						}
					}
					break;
				}
			}
		}
	}
	color(colour[0]);
}

void copy()//复制
{
	int x, y;
	char ch;
	//选择剪切部分字
	//清屏重新输出界面
	system("cls");
	choice = 0;
	toolbar();
	textOutput();
	//重新定位光标到文本框开头
	x = 0;
	y = 1;
	gotoxy(x, y);
	//存储文本指针头，过掉前六个密码指针
	struct wenzi *p, *t, *cp, *cq;//文本指针，剪贴板指针
	p = head;
	int start = 0;//0还没开始，1开始，2结束
	int num = 0;//行的字数
	for (int i = 0; i < 6; i++){
		p = p->next;
	}
	if (p != NULL){
		while (true){
			ch = getch();
			if (ch == 13){
				start++;
				if (start == 2){
					//结束，复制链表
					clipboard = (struct wenzi *)malloc(sizeof(struct wenzi));
					cp = clipboard;
					cp->next = NULL;
					while (t != p){
						cp->f[0] = t->f[0];
						cp->f[1] = t->f[1];
						cp->color = t->color;
						cp->font = t->font;
						cp->size = t->size;
						cq = cp;
						cp = (struct wenzi *)malloc(sizeof(struct wenzi));
						cq->next = cp;
						cp->previous = cq;
						t = t->next;
					}
					cp->f[0] = t->f[0];
					cp->f[1] = t->f[1];
					cp->color = t->color;
					cp->font = t->font;
					cp->size = t->size;
					cp->next = NULL;
					break;
				}
				else if (start == 1){
					//清楚剪贴板内容
					while (clipboard != NULL){
						cp = clipboard->next;
						free(clipboard);
						clipboard = cp;
					}
					//记录从开始
					t = p;
					color(colour[9]);
					printf("%c", p->f[0]);
					printf("%c", p->f[1]);
					gotoxy(x, y);
				}
			}
			else{
				ch = getch();
				switch (ch){
					//case 75://输入左键
					//	break;
				case 77://输入右键
					if (p->next != NULL){
						p = p->next;
						num++;
						x += 2;
						gotoxy(x, y);
						if (start == 1){
							if (p->f[0] == '\n'){
								num = 0;
								x = 0;
								y++;
								gotoxy(x, y);
							}
							else{
								printf("%c", p->f[0]);
								printf("%c", p->f[1]);
								gotoxy(x, y);
							}
						}
						if (num == 80){
							num = 0;
							x = 0;
							y++;
							gotoxy(x, y);
						}
					}
					break;
				}
			}
		}
	}
	color(colour[0]);
}

void paste()//粘贴
{
	if (clipboard != NULL){
		struct wenzi *p;
		p = head;
		while (p->next != NULL){
			p = p->next;
		}
		p->next = clipboard;
		clipboard = NULL;
	}
	else{
		gotoxy(0, 15);
		color(colour[15]);
		printf("\t\t\t\t\t剪贴板为空\n\t\t\t\t\t粘贴失败");
		color(colour[0]);
		char ch = getch();//定格画面
		system("cls");
	}
}

void del()//删除
{
	int x, y;
	char ch;
	//选择剪切部分字
	//清屏重新输出界面
	system("cls");
	choice = 0;
	toolbar();
	textOutput();
	//重新定位光标到文本框开头
	x = 0;
	y = 1;
	gotoxy(x, y);
	//存储文本指针头，过掉前六个密码指针
	struct wenzi *p, *d;//文本指针，剪贴板指针
	p = head;
	int start = 0;//0还没开始，1开始，2结束
	int num = 0;//行的字数
	for (int i = 0; i < 6; i++){
		p = p->next;
	}
	if (p != NULL){
		while (true){
			ch = getch();
			if (ch == 13){
				start++;
				if (start == 2){
					//结束，把链表拿出
					d->previous->next = p->next;
					p->next = NULL;
					while (d != NULL){
						p = d->next;
						free(d);
						d = p;
					}
					break;
				}
				else if (start == 1){
					//记录从开始
					d = p;
					color(colour[9]);
					printf("%c", p->f[0]);
					printf("%c", p->f[1]);
					gotoxy(x, y);
				}
			}
			else{
				ch = getch();
				switch (ch){
					//case 75://输入左键
					//	break;
				case 77://输入右键
					if (p->next != NULL){
						p = p->next;
						num++;
						x += 2;
						gotoxy(x, y);
						if (start == 1){
							if (p->f[0] == '\n'){
								num = 0;
								x = 0;
								y++;
								gotoxy(x, y);
							}
							else{
								printf("%c", p->f[0]);
								printf("%c", p->f[1]);
								gotoxy(x, y);
							}
						}
						if (num == 80){
							num = 0;
							x = 0;
							y++;
							gotoxy(x, y);
						}
					}
					break;
				}
			}
		}
	}
	color(colour[0]);
}



