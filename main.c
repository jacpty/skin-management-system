/*CSGO饰品购买程序*/

/*颜色对照表
0 = 黑色                8 = 灰色　　
1 = 蓝色                9 = 淡蓝色
2 = 绿色                10 = 淡绿色
3 = 湖蓝色              11 = 淡浅绿色
4 = 红色                12 = 淡红色
5 = 紫色                13 = 淡紫色
6 = 黄色                14 = 淡黄色
7 = 白色                15 = 亮白色*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<windows.h>
#define WIDTH 120 //程序界面宽度
#define STAR "******************************" //30个星号
#define LINE "------------------------------" //30条横线

typedef struct Item_mode //饰品模板
{
	char numbering[10]; //饰品编号
	char origin[10];    //武器产地
	char name[40];      //饰品名字
	int level;          //饰品稀有度
	int model;          //武器型号
	int price;          //饰品价格
	int inventory;      //库存数量
	int ordered;        //已订购数量

	struct Item_mode* next;  //指向下一结构体的指针
}Item;

void set_color(int level);        //改变窗口背景颜色和字体颜色
void output_split(char kind);     //打印欢迎界面中的分割线
void output_space(char str[]);    //打印欢迎界面中的空格
void output_command(char str[]);  //打印命令并将每个命令的首字母标红
void welcome(void);               //欢迎界面集成
void welcome_admin(void);         //管理员界面集成
void welcome_find(void);          //查找界面集成
void welcome_item(void);          //查找界面表头
void print_cart(void);            //输出购物车
int admin(void);				  //管理员账户登入
Item *add_item(int n);            //录入饰品
void write_info(Item *node);      //将录入的饰品保存到文件中
Item *read_info(void);            //从文件中读取饰品信息
void delete_info(Item *node, char numb[]); //删除文件中指定的饰品信息
void delete_cart(Item* node, char numb[]); //删除文件中指定的饰品信息
void write_cart(Item* node);      //将用户加入购物车的数据录入到文件中
Item* read_cart(void);            //从文件中读取购物车信息
void free_linked(Item *h);        //释放内存

void set_color(int level) //改变窗口背景颜色和字体颜色
{
	int number1, number2, number3, number4, number5;
	number1 = 240; //白底黑字
	number2 = 244; //白底红字
	number3 = 253; //白底粉字
	number4 = 245; //白底紫字
	number5 = 249; //白底蓝字

	if (level == 0)      //白底黑字
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), number1);
	else if (level == 1) //白底红字
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), number2);
	else if (level == 2) //白底粉字
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), number3);
	else if (level == 3) //白底紫字
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), number4);
	else if (level == 4) //白底蓝字
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), number5);
}

void output_split(char kind) //打印欢迎界面中的分割线
{
	int i;
	if (kind == 's')
	{
		for (i = 0; i < 4; i++)
			printf(STAR);
	}
	else if (kind == 'l')
	{
		for (i = 0; i < 4; i++)
			printf(LINE);
	}
}

void output_space(char str[]) //打印欢迎界面中的空格
{
	int i, front, len;
	len = strlen(str);
	front = (WIDTH - len) / 2;
	for (i = 0; i < front; i++)
		printf(" ");
}

void output_command(char str[]) //打印命令并将每个命令的首字母标红
{
	int i;
	for (i = 1; str[i] != '\0'; i++)
	{
		if (str[i - 1] == ' ' && isalpha(str[i])) //判断'str[i]'是否是命令的首字母
		{
			set_color(1);
			printf("%c", str[i]);
			set_color(0);
		}
		else
			printf("%c", str[i]);
	}
	printf("\n");
}

int admin(void) //管理员账户登入
{
	char temp[100] = { 0 };    
	char account[10] = { 0 };  //储存用户输入的账户
	char password[10] = { 0 }; //储存用户输入的密码
	int i, j, next = 0;

	//输入账号
	for (i = 0; i < 3; i++)
	{
		printf("请输入管理员账号: ");
		scanf_s("%s", temp, 10);
		strncpy(account, temp, 9); //裁取前9个字
		if (strcmp(account, "admin") == 0) //判断账户是否正确
		{
			system("cls");
			printf("请输入管理员密码: ");
			memset(temp, 0, 100);
			next++;
			break;
		}
		else
		{
			system("cls");
			printf("查无此账户，请重新输入！\n");
		}
	}
	//输入密码
	if (next != 1)
		return 0;
	else
	{
		for (j = 0; j < 3; j++)
		{
			scanf_s("%s", temp, 10);
			strncpy(password, temp, 9); //裁取前9个字
			if (strcmp(password, "admin") == 0) //判断密码是否正确
				return 1;
			else
			{
				system("cls");
				printf("密码错误，请重新输入！\n");
				printf("请输入密码: ");
			}
		}
	}
	return 0;
}

Item* add_item(int n) //录入饰品到文件中
{
	Item* head, * node, * end;
	int i;
	head = (Item*)malloc(sizeof(Item));
	end = head;

	for (i = 0; i < n; i++)
	{
		node = (Item*)malloc(sizeof(Item));
		printf("第%d件饰品\n", i+1);
		printf("编号: ");
		scanf_s("\n");
		gets_s(node->numbering, 10);         //编号
		printf("名字: ");
		gets_s(node->name, 40);              //名字
		printf("稀有度: ");
		scanf_s("%d", &node->level);         //稀有度
		printf("型号: ");
		scanf_s("%d", &node->model);         //型号
		printf("价格: ");
		scanf_s("%d", &node->price);         //价格
		printf("库存数量: ");
		scanf_s("%d", &node->inventory);     //库存数量
		printf("已订购数量: ");
		scanf_s("%d", &node->ordered);       //已订购数量
		printf("产地: ");
		scanf_s("\n");
		gets_s(node->origin, 10);            //产地
		printf("\n");
		write_info(node);
		end->next = node;
		end = node;
	}
	end->next = NULL;
	return head;
}

void write_info(Item *node) //将录入的饰品保存到文件中
{
	FILE* fw_item = fopen("Item_info.txt", "a");
	if (fw_item == NULL)
	{
		printf("打开文件失败");
		exit(1);
	}
	fprintf(fw_item, "%s\n", node->numbering);
	fprintf(fw_item, "%s\n", node->origin);
	fprintf(fw_item, "%s\n", node->name);
	fprintf(fw_item, "%d\n", node ->level);
	fprintf(fw_item, "%d\n", node->model);
	fprintf(fw_item, "%d\n", node->price);
	fprintf(fw_item, "%d\n", node->inventory);
	fprintf(fw_item, "%d\n", node->ordered);
	fclose(fw_item);
}

Item *read_info(void) //将读取到的数据写入到链表中
{
	FILE* fr_item = fopen("Item_info.txt", "r"); //打开文件
	Item* head, * node, * end;
	int i = 0;
	int last;
	char numbering[10] = { 0 }; //饰品编号
	char origin[10] = { 0 };    //武器产地
	char name[40] = { 0 };      //饰品名字
	char empty;

	if (fr_item == NULL)
	{
		printf("打开文件失败");
		exit(1);
	}
	head = (Item*)malloc(sizeof(Item));
	end = head;
	while (!feof(fr_item))
	{
		node = (Item*)malloc(sizeof(Item));
		//饰品编号
		fgets(numbering, 9, fr_item);
		last = strlen(numbering);
		numbering[last - 1] = '\0';
		strcpy(node->numbering, numbering);
		//武器产地
		fgets(origin, 9, fr_item);
		last = strlen(origin);
		origin[last - 1] = '\0';
		strcpy(node->origin, origin);
		//饰品名字
		fgets(name, 39, fr_item);
		last = strlen(name);
		name[last - 1] = '\0';
		strcpy(node->name, name);
		//饰品稀有度
		fscanf_s(fr_item, "%d", &node->level);
		//武器型号
		fscanf_s(fr_item, "%d", &node->model);
		//饰品价格
		fscanf_s(fr_item, "%d", &node->price);
		//库存数量
		fscanf_s(fr_item, "%d", &node->inventory);
		//已订购数量
		fscanf_s(fr_item, "%d", &node->ordered);
		fscanf_s(fr_item, "\t", &empty); //一个奇怪的原因需要在这去除掉一个制表符

		end->next = node;
		end = node;
	}
	end->next = NULL;
	fclose(fr_item);

	return head;
}

void write_cart(Item* node) //将录入的饰品保存到文件中
{
	FILE* fw_cart = fopen("Cart_info.txt", "a");
	if (fw_cart == NULL)
	{
		printf("打开文件失败");
		exit(1);
	}
	fprintf(fw_cart, "%s\n", node->numbering);
	fprintf(fw_cart, "%s\n", node->origin);
	fprintf(fw_cart, "%s\n", node->name);
	fprintf(fw_cart, "%d\n", node->level);
	fprintf(fw_cart, "%d\n", node->model);
	fprintf(fw_cart, "%d\n", node->price);
	fprintf(fw_cart, "%d\n", node->inventory);
	fprintf(fw_cart, "%d\n", node->ordered);
	fclose(fw_cart);
}

void delete_info(Item *node, char numb[10]) //删除文件中的数据
{
	FILE* fw_item = fopen("Item_info.txt", "w+");
	if (fw_item == NULL)
	{
		printf("打开文件失败");
		exit(1);
	}
	while (node->next != NULL)
	{
		node = node->next;
		if (strcmp(numb, node->numbering) != 0)
		{
			fprintf(fw_item, "%s\n", node->numbering);
			fprintf(fw_item, "%s\n", node->origin);
			fprintf(fw_item, "%s\n", node->name);
			fprintf(fw_item, "%d\n", node->level);
			fprintf(fw_item, "%d\n", node->model);
			fprintf(fw_item, "%d\n", node->price);
			fprintf(fw_item, "%d\n", node->inventory);
			fprintf(fw_item, "%d\n", node->ordered);
		}
		fclose(fw_item);
	}
}

void delete_cart(Item* node, char numb[10]) //删除购物车中的数据
{
	FILE* fw_cart = fopen("Cart_info.txt", "w+");
	if (fw_cart == NULL)
	{
		printf("打开文件失败");
		exit(1);
	}
	while (node->next != NULL)
	{
		node = node->next;
		if (strcmp(numb, node->numbering) != 0)
		{
			fprintf(fw_cart, "%s\n", node->numbering);
			fprintf(fw_cart, "%s\n", node->origin);
			fprintf(fw_cart, "%s\n", node->name);
			fprintf(fw_cart, "%d\n", node->level);
			fprintf(fw_cart, "%d\n", node->model);
			fprintf(fw_cart, "%d\n", node->price);
			fprintf(fw_cart, "%d\n", node->inventory);
			fprintf(fw_cart, "%d\n", node->ordered);
		}
		fclose(fw_cart);
	}
}

Item* read_cart(void) //将读取到的数据写入到链表中
{
	FILE* fr_cart = fopen("Cart_info.txt", "r"); //打开文件
	Item* head, * node, * end;
	int i = 0;
	int last;
	char numbering[10] = { 0 }; //饰品编号
	char origin[10] = { 0 };    //武器产地
	char name[40] = { 0 };      //饰品名字
	char empty;

	if (fr_cart == NULL)
	{
		printf("打开文件失败");
		exit(1);
	}
	head = (Item*)malloc(sizeof(Item));
	end = head;
	while (!feof(fr_cart))
	{
		node = (Item*)malloc(sizeof(Item));
		//饰品编号
		fgets(numbering, 9, fr_cart);
		last = strlen(numbering);
		numbering[last - 1] = '\0';
		strcpy(node->numbering, numbering);
		//武器产地
		fgets(origin, 9, fr_cart);
		last = strlen(origin);
		origin[last - 1] = '\0';
		strcpy(node->origin, origin);
		//饰品名字
		fgets(name, 39, fr_cart);
		last = strlen(name);
		name[last - 1] = '\0';
		strcpy(node->name, name);
		//饰品稀有度
		fscanf_s(fr_cart, "%d", &node->level);
		//武器型号
		fscanf_s(fr_cart, "%d", &node->model);
		//饰品价格
		fscanf_s(fr_cart, "%d", &node->price);
		//库存数量
		fscanf_s(fr_cart, "%d", &node->inventory);
		//已订购数量
		fscanf_s(fr_cart, "%d", &node->ordered);
		fscanf_s(fr_cart, "\t", &empty); //一个奇怪的原因需要在这去除掉一个制表符

		end->next = node;
		end = node;
	}
	end->next = NULL;
	fclose(fr_cart);

	return head;
}


void welcome(void) //欢迎界面
{
	char welcome1[] = { "欢迎来的 CSGO 饰品商店！" };
	char welcome2[] = { "现在你可以订购饰品！" };
	char welcome3[] = { "请输入标红的大写字母执行命令！" }; //提示进入命令的方法
	char welcome4[] = { " Admin:进入管理员账户    Find:查找饰品    Shoppingcart:查看购物车" }; //显示命令

	output_split('s');
	output_space(welcome1);  //第一句
	printf("%s\n", welcome1);
	output_space(welcome2);  //第二句
	printf("%s\n", welcome2);
	output_space(welcome3);  //第三句
	printf("%s\n", welcome3);
	output_space(welcome4);  //第四句
	output_command(welcome4);
	output_split('s');
	printf("\n");
}

void welcome_admin(void) //管理员权限欢迎界面
{
	char admin1[] = { "现在你已经拥有了管理员权限！" };
	char admin2[] = { "编号:1手枪 2重型武器 3冲锋枪 4步枪 5刀" };
	char admin3[] = { "稀有度:1隐秘 2保密 3受限 4军规级" };
	char admin4[] = { " Add:增加饰品    Delete:删除饰品" };

	output_split('s');
	output_space(admin1);
	printf("%s\n", admin1);
	output_space(admin2);
	printf("%s\n", admin2);
	output_space(admin3);
	printf("%s\n", admin3);
	output_space(admin4);
	output_command(admin4);
	output_split('s');
	printf("\n");
}

void welcome_find(void) //查找欢迎界面
{
	char find1[] = { "现在你可以按以下几种方式查询你想要的饰品！" };
	char find2[] = { " All:展示全部    Model:按型号查找    Name:按名字查找    Type:按类型查找" };
	output_split('s');
	output_space(find1);
	printf("%s\n", find1);
	output_space(find2);
	output_command(find2);
	output_split('s');
	printf("\n");
}

void welcome_item(void) //饰品表头
{
	system("cls");
	output_split('l');
	printf("编号\t名字\t\t\t\t型号\t价格\t库存数量\t已订购数量\t产地\n");
	output_split('l');
}

void free_linked(Item *h) //释放内存
{
	Item* temp;

	while (h->next != NULL)
	{
		temp = h->next;
		free(h);
		h = NULL;
		h = temp;
	}

	free(h);
	h = NULL;
}

/*主函数*/
int main(void)
{
	//将窗口改为白底黑字
	system("color F0"); //将全局背景改为白色，字体改为黑色
	set_color(0); //输出字体改为白底黑字
	welcome(); //欢迎界面

	//用户使用界面
	char command1, command2, command3;

	printf("请输入命令(输入'q'退出系统): ");
	while (scanf_s("\n%c", &command1, 1) == 1) //用\n来读取掉没用信息
	{
		if (command1 == 'q') //退出程序
			break;
		else if (command1 == 'A') //管理员账户
		{
			system("cls");
			if (admin())
			{
				system("cls");
				welcome_admin(); //管理员权限欢迎界面
				printf("请输入命令(输入'q'退出系统): ");
				while (scanf_s("\n%c", &command2, 1) == 1) //用\n来读取掉没用信息
				{
					if (command2 == 'q')
					{
						system("cls");
						welcome();
						break;
					}
					else if (command2 == 'A')
					{
						int frequency = 0;
						printf("需要录入饰品数据的数量: ");
						scanf_s("%d", &frequency);
						add_item(frequency);
						printf("请输入命令(输入'q'退出系统):");
					}
					else if (command2 == 'D')
					{
						Item* h;
						char numb[10];

						h = read_info();
						system("cls");
						welcome_item();
						int colour;

						while (h->next != NULL)
						{
							h = h->next;
							colour = h->level;
							set_color(colour);
							printf("%s\t", h->numbering);
							printf("%s\t\t", h->name);
							printf("%d\t", h->model);
							printf("%d\t", h->price);
							printf("%d\t\t", h->inventory);
							printf("%d\t\t", h->ordered);
							printf("%s\t\n", h->origin);
							set_color(0);
						}
						h = read_info();
						printf("请输入要删除饰品的编号(输入'q'退出系统): ");
						while (1)
						{
							if (scanf_s("\n%s", numb, 10) > 1)
							{
								printf("输入错误，请重新输入！\n");
								printf("请输入要删除饰品的编号(输入'q'退出系统): ");
							}
							else
								break;
						}
						delete_info(h, numb); //删除饰品

						printf("该饰品已经成功删除！(3 秒后退回到上一级界面)");
						Sleep(3000);
						system("cls");
						welcome_admin();
						printf("请输入命令(输入'q'退出系统): ");
					}
					else
						printf("查无此命令，请输入正确的命令: ");
				}
			}
			else
			{
				printf("您已经输错太多次了！\n");
				printf("请稍后再试!\n");
			}
		}
		else if (command1 == 'F') //查找物品
		{
			Item* h;
			char judge[10], quit[2] = "q";
			h = read_info();

			system("cls");
			welcome_find();
			printf("请输入命令(输入'q'退出系统): ");
			
			while (scanf_s("\n%c", &command3, 1) == 1)
			{
				if (command3 == 'q')
				{
					break;
					system("cls");
					welcome();
				}
				else if (command3 == 'A') //全部输出
				{
					welcome_item();
					int colour;

					while (h->next != NULL)
					{
						h = h->next;
						colour = h->level;
						set_color(colour);
						printf("%s\t", h->numbering);
						printf("%s\t\t", h->name);
						printf("%d\t", h->model);
						printf("%d\t", h->price);
						printf("%d\t\t", h->inventory);
						printf("%d\t\t", h->ordered);
						printf("%s\t\n", h->origin);
						set_color(0);
					}
					h = read_info(); //让指针重新指向head
					printf("请输入加入购物车的饰品编号(输入'q'退出系统): ");

				}
				else if (command3 == 'M') //型号
				{
					int colour, mo;

					printf("请输入你要查找的饰品型号: ");
					scanf_s("%d", &mo);
					welcome_item();
					while (h->next != NULL)
					{
						h = h->next;
						if (h->model == mo)
						{
							colour = h->level;
							set_color(colour);
							printf("%s\t", h->numbering);
							printf("%s\t\t", h->name);
							printf("%d\t", h->model);
							printf("%d\t", h->price);
							printf("%d\t\t", h->inventory);
							printf("%d\t\t", h->ordered);
							printf("%s\t\n", h->origin);
							set_color(0);
						}
					}
					h = read_info();
					printf("请输入加入购物车的饰品编号(输入'q'退出系统): ");
				}
				else if (command3 == 'N') //名字
				{
					int colour, length;
					char na[20];

					printf("请输入你要查找的饰品名字: ");
					scanf_s("%s", na, 20);
					length = strlen(na);
					na[length] = '\0';
					welcome_item();
					while (h->next != NULL)
					{
						h = h->next;
						if (strstr(h->name, na))
						{
							colour = h->level;
							set_color(colour);
							printf("%s\t", h->numbering);
							printf("%s\t\t", h->name);
							printf("%d\t", h->model);
							printf("%d\t", h->price);
							printf("%d\t\t", h->inventory);
							printf("%d\t\t", h->ordered);
							printf("%s\t\n", h->origin);
							set_color(0);
						}
					}
					h = read_info();
					printf("请输入加入购物车的饰品编号(输入'q'退出系统): ");
				}
				else if (command3 == 'T') //类型
				{
					int colour;
					char ty;

					printf("请输入你要查找的饰品型号: ");
					scanf_s("\n%c", &ty, 1);
					welcome_item();
					while (h->next != NULL)
					{
						h = h->next;
						if (h->numbering[0] == ty)
						{
							colour = h->level;
							set_color(colour);
							printf("%s\t", h->numbering);
							printf("%s\t\t", h->name);
							printf("%d\t", h->model);
							printf("%d\t", h->price);
							printf("%d\t\t", h->inventory);
							printf("%d\t\t", h->ordered);
							printf("%s\t\n", h->origin);
							set_color(0);
						}
					}
					h = read_info();
					printf("请输入加入购物车的饰品编号(输入'q'退出系统): ");

				}
				else
					printf("查无此命令，请输入正确的命令: ");

					int len, count = 0;
					scanf_s("\n%s", judge, 10); //输入加入购物车饰品的编号
					len = strlen(judge);
					judge[len] = '\0'; //处理字符串防止报错

					while (h->next != NULL)
					{
						h = h->next;
						if (strcmp(h->numbering, judge) == 0) //判断是否有这个饰品
						{
							count += 1;
							write_cart(h);
							break;
						}
					}
					h = read_info();

					if (strcmp(judge, quit) == 0) //返回主界面
					{
						system("cls");
						welcome_find();
					}
					else if (count == 1) //饰品存在
					{
						printf("已添加到购物车！(3 秒后将回到主界面)");
						Sleep(3000);
						system("cls");
						welcome_find();
					}
					else //饰品不存在
					{
						printf("查无此商品！(3 秒后将回到主界面)");
						Sleep(3000);
						system("cls");
						welcome_find();
					}
					count = 0;

					printf("请输入命令(输入'q'退出系统): ");
					
			}
		}
		else if (command1 == 'S') //查看购物车 
		{
			Item* h;
			h = read_cart();

			while (1)
			{
				int colour;
				char order;
				welcome_item();
				while (h->next != NULL)
				{
					h = h->next;
					colour = h->level;
					set_color(colour);
					printf("%s\t", h->numbering);
					printf("%s\t\t", h->name);
					printf("%d\t", h->model);
					printf("%d\t", h->price);
					printf("%d\t\t", h->inventory);
					printf("%d\t\t", h->ordered);
					printf("%s\t\n", h->origin);
					set_color(0);
				}
				h = read_cart();

				printf("\n是否下单(Y/N)(输入'q'退出系统)?\n");
				scanf_s("\n%c", &order, 1);

				if (order == 'q')
					break;
				else if (order == 'Y')
				{
					char country[20], province[20], city[20], area[20], street[20], address[100];
					printf("请输入你的地址：\n");
					printf("国家：");
					scanf_s("%s", country, 20);
					printf("省份：");
					scanf_s("%s", province, 20);
					printf("城市：");
					scanf_s("%s", city, 20);
					printf("所在地区：");
					scanf_s("%s", area, 20);
					printf("街道：");
					scanf_s("%s", street, 20);
					printf("详细地址：");
					scanf_s("%s", address, 100);

					system("cls");
					int colour;
					char order;
					welcome_item();
					while (h->next != NULL)
					{
						h = h->next;
						colour = h->level;
						set_color(colour);
						printf("%s\t", h->numbering);
						printf("%s\t\t", h->name);
						printf("%d\t", h->model);
						printf("%d\t", h->price);
						printf("%d\t\t", h->inventory);
						printf("%d\t\t", h->ordered);
						printf("%s\t\n", h->origin);
						set_color(0);
					}
					h = read_cart();

					printf("\n%s%s%s%s%s%s", country, province, city, area, street, address); //打印地址
					printf("\n是否下单(Y/N)?\n");
					while (scanf_s("\n%c", &order, 1) == 1)
					{
						if (order == 'Y')
						{
							printf("正在创建订单...\n");
							Sleep(1000);
							printf("对不起，你的账户中没有足够的金额！(5 秒后回到主界面)"); //你莫得钱
							Sleep(5000);
							break;
						}
						else if (order == 'N')
							break;
						else
							printf("请输入正确的命令！\n");
					}

				}
				else if (order == 'N')
				{
					Item* h;
					char numb[10];

					h = read_cart();
					system("cls");
					welcome_item();
					int colour;

					while (h->next != NULL)
					{
						h = h->next;
						colour = h->level;
						set_color(colour);
						printf("%s\t", h->numbering);
						printf("%s\t\t", h->name);
						printf("%d\t", h->model);
						printf("%d\t", h->price);
						printf("%d\t\t", h->inventory);
						printf("%d\t\t", h->ordered);
						printf("%s\t\n", h->origin);
						set_color(0);
					}
					h = read_cart();
					printf("请输入要移出购物车的饰品编号(输入'q'退出系统): ");
					while (1)
					{
						if (scanf_s("\n%s", numb, 10) > 1)
						{
							printf("输入错误，请重新输入！\n");
							printf("请输入要移出购物车的饰品编号(输入'q'退出系统): ");
						}
						else
							break;
					}
					delete_cart(h, numb); //删除饰品

					printf("该饰品已经成功删除！(3 秒后退回到上一级界面)");
					Sleep(3000);
					system("cls");
					welcome_admin();
					printf("请输入命令(输入'q'退出系统): ");

				}

				system("cls");
			}
		}
		else
		{
			printf("查无此命令，请输入正确的命令: ");
		}

		system("cls");
		welcome();
		printf("请输入命令(输入'q'退出系统): ");
	}
	printf("谢谢惠顾，希望您下次再来！");

	return 0;
}