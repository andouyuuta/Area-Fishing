#pragma once
#include<DxLib.h>

class Water
{
public:
	Water(void);
	~Water(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
private:
	VECTOR pos_;
	int watermodel_;
	int waterSoundHundle;	
};