#pragma once
#include <DxLib.h>

class Water
{
public:
	Water(void);
	~Water(void);
	void Init(void);		// ����������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��
	void Release(void);		// �������
private:
	VECTOR pos_;			// ���W
	VECTOR scale_;			// �傫��
	int waterModel_;		// �����f��
};