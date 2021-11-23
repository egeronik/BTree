#pragma once
#ifndef BINWORK
#define BINWORK
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;


struct record {
	int key = 0;
	int code = 0;
	char data[20];
};
/// <summary>
/// �������� ��������� ����� �� ���� ����������
/// </summary>
/// <param name="nametf">��� ���������� �����</param>
/// <param name="namebf">��� ��������� ����� ������� ����� ������</param>
/// <returns></returns>
int CreateBinFile(string nametf, string namebf);

/// <summary>
/// ������� ��������� �����
/// </summary>
/// <param name="namebf">��� �����</param>
/// <returns>������ ���� ���������� �� ���������</returns>
vector<record> outBinFile(string namebf);

/// <summary>
/// ������ ����� ������ �� ���������
/// </summary>
/// <param name="namebf">��� �����</param>
/// <param name="pos">����� ������ ������� � 0</param>
/// <returns>������</returns>
record readRecordFromBin(string namebf, int pos);

/// <summary>
/// �������� ������
/// </summary>
/// <param name="namebf">��� �����</param>
/// <param name="pos">������� ������ ������� � 0</param>
void deleteRecord(string namebf, int pos);

/// <summary>
/// ���������� ������ � ����
/// </summary>
/// <param name="namebf">��� ��������� �����</param>
/// <param name="r">������ ����������� ��� ����������</param>
void addRecord(string namebf, record r);
#endif // !BINWORK
