#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// strcmp ���� ��� �����
// malloc �������� �̱��� �����

unsigned char random() {

}

unsigned char slide(unsigned char a, int pos) {

}

unsigned char deSlide(unsigned char a, int pos) {

}

const int MIX_TURN = 16;
const int RANDOM_USING_COUNT = MIX_TURN * 2;

void byteSpliter(char* data, int size) {
	for (int i = 0; i < size; i++)
		for (int k = 0; k < MIX_TURN; k++) {
			data[i] = slide(data[i], random());
			data[i] = data[i] ^ random();
		}
}

unsigned char randoms[RANDOM_USING_COUNT];
void byteRestorer(char* data, int size) {
	for (int i = 0; i < size; i++) {
		for (int k = 0; k < RANDOM_USING_COUNT; k++)
			randoms[k] = random();
		for (int k = RANDOM_USING_COUNT - 1; k >= 0;) {
			data[i] = data[i] ^ randoms[k--];
			data[i] = deSlide(data[i], randoms[k--]);
		}
	}
}

void byteLocker(FILE* inputfp, FILE* outputfp) {
	int dataSize = 1024 * 1024 * 256; // 256MiB
	char* data = (char*)malloc(dataSize * sizeof(char));
	int readSize = 0;
	int writeSize = 0;

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
		
		byteSpliter(data, readSize);

		writeSize = fwrite(data, sizeof(char), readSize, outputfp);
		if (writeSize != readSize) {
			// ���⼭ ��������� ����߸� �������ϰԲ� ������
			printf("������ �߻��Ͽ� �����մϴ�.");
			fclose(inputfp);
			fclose(outputfp);
			exit(1);
		}
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
		scanf_s("%s", inputPath, sizeof(inputPath));
	} while ((fopenValue = fopen_s(&inputFile, "", "r")) != 0);
	
	fopenValue = 0;
	strcmpValue = 1;
	do {
		if (fopenValue != 0)
			printf("�Է��� ����� ������ �� �� �����ϴ�!\n");
		if (strcmpValue == 0)
			printf("��������� �Է����ϰ� ���� �� �����ϴ�!\n");
		printf("��������� ��θ� �Է��ϼ��� : \n  ");
		scanf_s("%s", outputPath, sizeof(outputPath));
	} while ((strcmpValue = strcmp(inputPath,outputPath)) == 0 
		|| (fopenValue = fopen_s(&outputFile, "", "w")) != 0);

	printf("���� ��ȣ Ű�� �Է��ϼ��� :\n  ");
	scanf_s("%s",seedString,sizeof(seedString));

	byteLocker(inputFile, outputFile);

	printf("������ ��ȣȭ�� ���������� �Ϸ��߽��ϴ�.");
	fclose(inputFile);
	fclose(outputFile);

	return 0;
}