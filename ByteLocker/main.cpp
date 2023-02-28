#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "locker.h"

// strcmp 버퍼 벗어남 취약요소
// malloc 성공실패 미구분 취약요소
// 문자열 받을때 마지막이 null이 아닐 수 있는 취약요소

int KBCount;
void byteLocker(FILE* inputfp, FILE* outputfp, char* seedString, int seedLen, byteOperator* byteFunction) {
	int dataSize = 1024 * 1024 * 256; // 256MiB
	char* data = (char*)malloc(dataSize * sizeof(char));
	int readSize = 0;
	int writeSize = 0;

	setRandomSeed(seedString, seedLen);

	KBCount = 0;
	while (true) {
		readSize = fread(data, sizeof(char), dataSize, inputfp);
		if (readSize != dataSize)
			if (feof(inputfp) == 0 || ferror(inputfp) != 0) {
				// 여기서 강제종료로 벗어나야만 정상동작하게끔 설계함
				printf("문제가 발생하여 종료합니다.");
				fclose(inputfp);
				fclose(outputfp);
				exit(1);
			}
		if (readSize == 0)
			break;
		
		(*byteFunction)(data, readSize);

		writeSize = fwrite(data, sizeof(char), readSize, outputfp);
		if (writeSize != readSize) {
			// 여기서 강제종료로 벗어나야만 정상동작하게끔 설계함
			printf("문제가 발생하여 종료합니다.");
			fclose(inputfp);
			fclose(outputfp);
			exit(1);
		}

		KBCount += readSize;
		printf("%dByte (총 %.2fKB) 완료\n", readSize, KBCount/1024.0);
	}

	free(data);
}

int main() {

	FILE* inputFile;
	FILE* outputFile;
	char inputPath[1024];
	char outputPath[1024];
	char seedString[1024];

	int fopenValue = 0;
	int strcmpValue = 1;
	do {
		if (fopenValue != 0)
			printf("입력한 경로의 파일을 열 수 없습니다!\n");
		printf("입력파일의 경로를 입력하세요 : \n  ");
		gets_s(inputPath);
		fflush(stdin);
	} while ((fopenValue = fopen_s(&inputFile, inputPath, "rb")) != 0);
	
	fopenValue = 0;
	strcmpValue = 1;
	do {
		if (fopenValue != 0)
			printf("입력한 경로의 파일을 열 수 없습니다!\n");
		if (strcmpValue == 0)
			printf("출력파일은 입력파일과 같을 수 없습니다!\n");
		printf("출력파일의 경로를 입력하세요 : \n  ");
		gets_s(outputPath);
		fflush(stdin);
	} while ((strcmpValue = strcmp(inputPath,outputPath)) == 0 
		|| (fopenValue = fopen_s(&outputFile, outputPath, "wb")) != 0);

	printf("파일 암호 키를 입력하세요 :\n  ");
	gets_s(seedString);
	fflush(stdin);

	int value;
	printf("파일을 암호화하려면 1을, 파일을 복호화하려면 0(또는 다른 값)을 입력하세요 :\n  ");
	scanf_s("%d",&value);

	byteOperator byteFunc;
	if (value == 1)
		byteFunc = byteSpliter;
	else
		byteFunc = byteRestorer;

	byteLocker(inputFile, outputFile, seedString, strlen(seedString), &byteFunc);

	printf("데이터 암/복호화를 성공적으로 완료했습니다.\n\n");
	fclose(inputFile);
	fclose(outputFile);

	system("pause");

	return 0;
}