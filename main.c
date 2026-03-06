#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define N 20
#define M 10
#define MAXNAME 50
#define MAXSONG 100


//创建选手结构体
typedef struct player{
	int number;
	char name[MAXNAME];
	char song[MAXSONG];
	double score[M];
	double finalScore;
}PLAYER;


//创建评委结构体
typedef struct judge{
	int id;
	float level;
}JUDGE;



//声明各项函数
int menu(void);
int playerAmount(void);
int judgeAmount(void);
int readSongs(char songs[][MAXSONG]);
int enrolling(PLAYER players[],int n,char songs[][MAXSONG], int songCount);
void scoring(PLAYER players[],int n,int m);
void playersRanking(PLAYER players[],int n);
void judgeRanking(PLAYER players[],int n,JUDGE judges[],int m);
void readwin(char win[][100],PLAYER players[],int n);
void songlist(int songCount,char songs[][MAXSONG]);
int isSongInList(char song[], char songs[][MAXSONG], int songCount);

//主函数定义
int main() {
	int n=N,m=M;
	PLAYER players[N];
	JUDGE judges[M];
	char songs[100][MAXSONG];
	int songCount=0;
	for (int i=0;i<M;i++){
		judges[i].id=i+1;
		judges[i].level=0.0;
	}
	
	songCount=readSongs(songs);
	char win[20][100];
    int hasRegistered=0;
	while(1){
		switch (menu()) {
			case 1:
				n=playerAmount();
				break;
			case 2:
				m=judgeAmount();
				break;
			case 3:
				hasRegistered=enrolling(players,n,songs,songCount);
				break;
			case 4:
				if (!hasRegistered) printf("请先注册选手！\n");
				else scoring(players,n,m);
				break;
			case 5:
				playersRanking(players,n);
				break;
			case 6:
				judgeRanking(players,n,judges,m);
				break;
			case 7:
				readwin(win,players,n);
				break;
		
		
			case 0:
				printf("Over! Thank you!\n");
				exit(0);
			default:
				//TODO
				break;
		}
	}
	return 0;
}




//菜单函数定义
int menu(void){
	int itemSelected;
	printf("\n=========Student Score Analysis=========\n");
	printf("              1. Set Player Amout\n");
	printf("              2. Set Judge Amount\n");
	printf("              3. Enrolling\n");
	printf("              4. Scoring\n");
	printf("              5. Player Ranking\n");
	printf("              6. Judge Ranking\n");
	printf("              7. Awarding\n");

	printf("              0. Exit\n");
	printf("======================================\n");
	
	printf("Your choice: ");
	scanf("%d", &itemSelected);
	
	    return itemSelected;
	}
	
//参赛选手，裁判人数设定的函数定义
int playerAmount(){
	int n;
	printf("请输入参赛选手的人数：");
	scanf("%d",&n);
	return n;
}

int judgeAmount(){
	int m;
	printf("请输入裁判的人数：");
		scanf("%d",&m);
	while (m<3){
		printf("请重新输入，不得小于3人\n");
		scanf("%d",&m);
	}

	return m;
}

//读取歌曲名字的函数定义
int readSongs(char songs[][MAXSONG]){
	FILE *fp;
	int count=0;
	
	fp=fopen("songs.txt","r");
	printf("正在读取歌曲列表...");
	if (fp == NULL) {
	    printf("错误：无法打开歌曲文件！\n");
	    return 0;  
	    
	}
	while (count<100&&fgets(songs[count],MAXSONG,fp)!=NULL){
		int len=strlen(songs[count]);
		if (len>0 && songs[count][len-1]=='\n') songs[count][len-1]='\0';
		count++;
	}
	fclose(fp);
	
	printf("成功读取%d首歌曲",count);
	printf("可选歌曲列表：\n");
				for (int j=0;j<count;j++){
					printf("%d. %s\n",j+1,songs[j]);
				}
	return count;
}

//选手报名的函数定义
int enrolling(PLAYER players[],int n,char songs[][MAXSONG], int songCount){
	
	printf("\n====选手注册====\n");
	int hasRegistered=0;
	for(int i=0;i<n;i++){
		
		players[i].number=i+1;
		printf("请输入姓名：");
		scanf("%s",players[i].name);
		
		
		
		printf("请输入参赛歌曲:");
		scanf("%s",players[i].song);
		
		
		while(!isSongInList(players[i].song, songs, songCount)){
		printf("错误：歌曲 '%s' 不在备选列表中！\n", players[i].song);
		printf("  请重新从上面的列表中选择一首歌：\n");	
		scanf("%s",players[i].song);
		}
        
        	printf("歌曲选择成功！\n");
			printf("注册成功!\n%d号选手%s：%s\n",players[i].number,players[i].name,players[i].song);
			hasRegistered++;
		
	}
	return hasRegistered;
}

//打分函数定义
void scoring(PLAYER players[],int n,int m){
	printf("=== 评委打分环节 ===\n");
	
	for (int i=0;i<n;i++){
		printf("请为第%d位选手%s:%s打分\n",players[i].number,players[i].name,players[i].song);
		for (int j=0;j<m;j++){
			printf("第%d位评委打分\n",j+1);
			scanf("%lf", &players[i].score[j]);
			while (players[i].score[j] < 0 || players[i].score[j] > 10) {
			    printf("分数必须在0-10之间，请重新输入：");
			    scanf("%lf", &players[i].score[j]);
			}
		}
		
		double maxscore=players[i].score[0];
		double minscore=players[i].score[0];
		double sum=players[i].score[0];
		
		for(int j=1;j<m;j++){
			double score=players[i].score[j];
			sum+=score;
			
			if (score>maxscore) maxscore=score;
			if (score<minscore) minscore=score;
		}
		
		sum=sum-maxscore-minscore;
		players[i].finalScore=sum/(m-2);
		
		printf("最高分：%.2f,最低分：%.2f\n",maxscore,minscore);
		printf("第%d位选手%s的最终得分为：%.2f\n",players[i].number,players[i].name,players[i].finalScore);
	}
	
}


//选手排名函数定义
void playersRanking(PLAYER players[],int n){
	printf("\n====选手得分排名====\n");
	
	for (int i=0;i<n-1;i++){
		for (int j=0;j<n-1-i;j++){
			if (players[j].finalScore<players[j+1].finalScore)
			{
				PLAYER temp=players[j];
				players[j]=players[j+1];
				players[j+1]=temp;
			}
		}
	}
	
printf("\n排名  编号   姓名                最终得分\n");
printf("---------------------------------\n");

for (int i=0;i<n;i++){
	printf("%2d    %3d    %-15s    %6.2f\n",i+1,players[i].number,players[i].name,players[i].finalScore);
	
}
}

//裁判排名函数定义
void judgeRanking(PLAYER players[],int n,JUDGE judges[],int m){
	printf("\n====评委评分水平排名====\n");
	for(int j=0;j<m;j++){
		double total=0.0;
		for (int i=0;i<n;i++){
		double error=fabs(players[i].score[j]-players[i].finalScore);
		total+=error;
		}
		double ave=total/n;
		judges[j].level=1.0/(1.0+ave);
	}
	
	

	for (int i=0;i<m;i++){
		for (int j=0;j<m-1-i;j++){
			if (judges[j].level<judges[j+1].level){
				JUDGE temp=judges[j];
				judges[j]=judges[j+1];
				judges[j+1]=temp;
			}
		}
	}
	
	printf("\n排名  评委ID   评分水平   评价\n");
	printf("----------------------------------------\n");
	for (int i=0;i<m;i++){
		char *level;
		if(judges[i].level>=0.9) level="优秀";
		else if (judges[i].level>=0.7) level="良好";
		else if (judges[i].level>=0.5) level="一般";
		else level="待提高";
		printf("%2d    %4d     %.4f    %s\n",i+1,judges[i].id,judges[i].level,level);
		
	}
}

void readwin(char win[][100],PLAYER players[],int n){
	FILE *fp=fopen("youwin.txt","r");
	int line=0;
	while(line<10 && fgets(win[line],100,fp)!=NULL){
		printf("%s",win[line]);
		line++;
	}
	fclose(fp);
	
	
	int index = 0;
    for (int i = 1; i < n; i++) {
        if (players[i].finalScore > players[index].finalScore) {
            index = i;
        }
    }	




	    printf("\n           冠军信息 \n");
	    printf("═══════════════════════════════════════════\n");
	    printf("   编号：%d\n", players[index].number);
	    printf("   姓名：%s\n", players[index].name);
	    printf("   参赛歌曲：《%s》\n", players[index].song);
	    printf("═══════════════════════════════════════════\n");
	    
}


// 检查歌曲是否在列表中
int isSongInList(char song[], char songs[][MAXSONG], int songCount) {
    for (int i = 0; i < songCount; i++) {
        if (strcmp(song, songs[i]) == 0) {
            return 1;  
        }
    }
    return 0;  
}
