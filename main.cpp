#include<stdio.h>
#include<direct.h>
#include<io.h>
#include<string.h>
#include<iostream>
#include <windows.h>

using namespace std;

int compareArray(unsigned char *array1, int array1Size, unsigned char *array2, int array2Size);
int modifyPE(FILE *fp, unsigned char *oriData, int oriDataSize, unsigned char *newData, int newDataSize);

void main() {


	/**
	 * (改变使用权限)要替换的指令
	 * movzx rax, byte ptr ds:[rcx+0x600]
	 * ret
	 */
	unsigned char oriData[] = {0x48, 0x0F, 0xB6, 0x81, 0x00,
		0x06, 0x00, 0x00, 0xC3, 0xCC,
		0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0xCC};

	/**
	 * (改变使用权限)新的指令
	 * add rcx, 0x600
	 * mov word ptr ds:[ecx], 0x500
	 * xor eax,eax
	 * ret
	 */
	unsigned char crackData[] = {0x48, 0x81, 0xC1, 0x00, 0x06,
		0x00, 0x00, 0x67, 0x66, 0xC7,
		0x01, 0x00, 0x05, 0x33, 0xC0,
		0xC3};

	/**
	 * (去掉提示框)要替换的指令
	 **/
    unsigned char oriJmpData[] = {0x06, 0x00, 0x00, 0x00, 0x0F,
		0x84, 0x3C, 0x01, 0x00, 0x00, 
	    0x48, 0x8B, 0x4D, 0x30, 0xE8};

	/**
	 * (去掉提示框)新的指令
	 **/
    unsigned char crackJmpData[] = {0x06, 0x00, 0x00, 0x00, 0xE9,
		0x3D, 0x01, 0x00, 0x00, 0x90,
	0x48, 0x8B, 0x4D, 0x30, 0xE8};

	/**
	 * 获取当前工作目录
	 */
	char path[80];
	getcwd(path, sizeof(path));

	/**
	 * 查找当前程序远行的目录下是否存在BCompare.exe文件
	 */
	struct _finddata_t data;
	long hnd = _findfirst("BCompare.exe", &data);
	int nRet = (hnd < 0) ? -1 : 1;
	if(nRet >= 0) {
		printf("找到BCompare.exe，开始破解... \n");
		strcat(path, "\\");
		strcat(path, data.name);
		FILE *fp;
		if((fp = fopen(path, "rb+")) == NULL) {
		    printf("file cannot open \n");
			exit(0);
		} 

        printf("开始查询权限代码，请稍后...");
		int mRes1 = modifyPE(fp, oriData, sizeof(oriData), crackData, sizeof(crackData));
        printf("开始去掉提示框，请稍后...");
        int mRes2 = modifyPE(fp, oriJmpData, sizeof(oriJmpData), crackJmpData, sizeof(crackJmpData));


		if(mRes1 == 0 && mRes2 == 0) {
            printf("破解成功 \n");
		} else {
		    printf("破解失败 \n");
		}
		fclose(fp);
	} else {
	    printf("没有找到BCompare.exe, 请将程序拷贝到BCompare.exe目录下 \n");
	}
	_findclose(hnd);//关闭当前句柄
	system("pause");
}


/**
 *
 * 比较字符数组是否相符
 */
int compareArray(unsigned char *array1, int array1Size, unsigned char *array2, int array2Size) {
	if(array1Size != array2Size) {
	    return -1;
	}
	int i;
	for(i = 0; i < array1Size; i ++) {
		if(array1[i] != array2[i]) {
		    return -1;
		}
	}
	return 0;
}

/**
 *
 * 查询PE一段二进制数据并修改成另一段二进制数据
 */
int modifyPE(FILE *fp, unsigned char *oriData, int oriDataSize, unsigned char *newData, int newDataSize) {
        unsigned char *curReadData = (unsigned char *)malloc(oriDataSize);
		fseek(fp, 0, SEEK_SET);
		int rNum = fread(&curReadData[0], sizeof(char), oriDataSize, fp);
		while(rNum > 0) {
		    int cRes = compareArray(curReadData, rNum, oriData, oriDataSize);
			if(cRes == 0) {
				printf("已经找到待修改位置 \n");

			    //如果数据相等，将文件位置指示器设置到上次读取的位置
				fseek(fp, -rNum, SEEK_CUR);
				
				//写入新数据
                fwrite(&newData[0], sizeof(char), newDataSize, fp);

				free(curReadData);
				return 0;
			} else {
				//如果数据不相等，将文件位置指示器设置到上次读取的位置+1字节，并继续读取arraySize个字节的数据
                fseek(fp, -rNum + 1, SEEK_CUR);
			    rNum = fread(&curReadData[0], sizeof(char), oriDataSize, fp);
			}
		}
		free(curReadData);
        printf("未找到待修改位置 \n");
		return -1;
}