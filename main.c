#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

char addressMail[] = "ungvietanh120799@gmail.com";
int j;
pthread_mutex_t mutexLock;
pthread_t threadUpdateInfo[2];
pthread_t threadSendMail[2];

typedef struct student {
   char  name[20];
   float cpa;
   unsigned int toeicScore;
   unsigned int creditPass;
   unsigned int tuitionFee;
};

struct student va;

void EditInfo(void)
{
	printf("Nhap Ten Sinh Vien %s:",va.name);
	printf("\nNhap Diem CPA: ");
	scanf("%f", &va.cpa);

	printf("Nhap Diem TOEIC: ");
	scanf("%d", &va.toeicScore);

	printf("Nhap So Tin Chi Dat: ");
	scanf("%d", &va.creditPass);

	printf("Nhap Hoc Phi: ");
	scanf("%d", &va.tuitionFee);
}

void *update(void*arg){
	pthread_mutex_lock(&mutexLock);
	printf("Job %d started\n", pthread_self());
    j = (int)pthread_self();
	EditInfo();
	printf("Ban da nhap CPA: %f", va.cpa);
	printf(" - TOEIC: %d", va.toeicScore);
	printf(" - So Tin Chi Dat: %d", va.creditPass);
	printf(" - Hoc Phi: %d\n", va.tuitionFee);
	printf("Job %d finished \n", pthread_self());
    printf("--------------------------\n\n");
	pthread_mutex_unlock(&mutexLock);
	return NULL;
}

void *sendmail(void *arg)
{
    pthread_mutex_lock(&mutexLock);
	char cmd[100];
    sprintf(cmd, "echo %s | mail -s \" Thread %d da cap nhat thong tin \" %s", "", j, addressMail);
    system(cmd);
    pthread_mutex_unlock(&mutexLock);
    return NULL;
}

int main(){
	int i = 0;
	int err;
	strcpy(va.name, "Ung Viet Anh");
	//declare mutex
    if (pthread_mutex_init(&mutexLock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
	while(i < 2)
    {
        err = pthread_create(&(threadUpdateInfo[i]), NULL, &update, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        err = pthread_create(&(threadSendMail[i]), NULL, &sendmail, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        i++;
    }
    pthread_join(threadUpdateInfo[0], NULL);
    pthread_join(threadUpdateInfo[1], NULL);
	pthread_join(threadSendMail[0], NULL);
    pthread_join(threadSendMail[1], NULL);
    pthread_mutex_destroy(&mutexLock); //destroy mutex

    return 0;
}

