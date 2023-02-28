#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "locker.h"

// strcmp ���� ��� �����
// malloc �������� �̱��� �����
// ���ڿ� ������ �������� null�� �ƴ� �� �ִ� �����

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
				// ���⼭ ��������� ����߸� �������ϰԲ� ������
				printf("������ �߻��Ͽ� �����մϴ�.");
				fclose(inputfp);
				fclose(outputfp);
				exit(1);
			}
		if (readSize == 0)
			break;
		
		(*byteFunction)(data, readSize);

		writeSize = fwrite(data, sizeof(char), readSize, outputfp);
		if (writeSize != readSize) {
			// ���⼭ ��������� ����߸� �������ϰԲ� ������
			printf("������ �߻��Ͽ� �����մϴ�.");
			fclose(inputfp);
			fclose(outputfp);
			exit(1);
		}

		KBCount += readSize;
		printf("%dByte (�� %.2fKB) �Ϸ�\n", readSize, KBCount/1024.0);
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
			printf("�Է��� ����� ������ �� �� �����ϴ�!\n");
		printf("�Է������� ��θ� �Է��ϼ��� : \n  ");
		gets_s(inputPath);
		fflush(stdin);
	} while ((fopenValue = fopen_s(&inputFile, inputPath, "rb")) != 0);
	
	fopenValue = 0;
	strcmpValue = 1;
	do {
		if (fopenValue != 0)
			printf("�Է��� ����� ������ �� �� �����ϴ�!\n");
		if (strcmpValue == 0)
			printf("��������� �Է����ϰ� ���� �� �����ϴ�!\n");
		printf("��������� ��θ� �Է��ϼ��� : \n  ");
		gets_s(outputPath);
		fflush(stdin);
	} while ((strcmpValue = strcmp(inputPath,outputPath)) == 0 
		|| (fopenValue = fopen_s(&outputFile, outputPath, "wb")) != 0);

	printf("���� ��ȣ Ű�� �Է��ϼ��� :\n  ");
	gets_s(seedString);
	fflush(stdin);

	int value;
	printf("������ ��ȣȭ�Ϸ��� 1��, ������ ��ȣȭ�Ϸ��� 0(�Ǵ� �ٸ� ��)�� �Է��ϼ��� :\n  ");
	scanf_s("%d",&value);

	byteOperator byteFunc;
	if (value == 1)
		byteFunc = byteSpliter;
	else
		byteFunc = byteRestorer;

	byteLocker(inputFile, outputFile, seedString, strlen(seedString), &byteFunc);

	printf("������ ��/��ȣȭ�� ���������� �Ϸ��߽��ϴ�.\n\n");
	fclose(inputFile);
	fclose(outputFile);

	system("pause");

	return 0;
}