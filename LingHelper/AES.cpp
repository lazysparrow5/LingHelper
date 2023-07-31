#include "pch.h"
#include "AES.h"

Password::Password()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			plaintext[i][j] = 0;
		}
	}
}

Password::~Password()
{
}

Password::Password(uint8_t num)
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			this->plaintext[i][j] = num;
		}
	}
}

Password::Password(Password &pwd)
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			this->plaintext[i][j] = pwd.plaintext[i][j];
		}
	}
}

Password::Password(char *password)
{
	int len = strlen(password);
	if (len < SIZE)
	{
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++)
			{
				this->plaintext[j][i] = (SIZE - len);
			}
		}
		for (int i = 0; i < len; i++)
		{
			this->plaintext[i % 4][i / 4] = *(password++);
		}
		return;
	}
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			this->plaintext[j][i] = *(password++);
		}
	}
}

Password::Password(const uint8_t *password, int len)
{
	if (len < SIZE)
	{
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++)
			{
				this->plaintext[j][i] = (SIZE - len);
			}
		}
		for (int i = 0; i < len; i++)
		{
			this->plaintext[i % 4][i / 4] = *(password++);
		}
		return;
	}
	else
	{
		for (int i = 0; i < SIZE; i++)
		{
			this->plaintext[i % 4][i / 4] = *(password++);
		}
	}
}

uint8_t *Password::GetChar()
{
	uint8_t *pwd = new uint8_t[SIZE + 1];
	for (int i = 0; i < SIZE; i++)
	{
		pwd[i] = this->plaintext[i % 4][i / 4];
	}
	pwd[SIZE] = '\0';
	return pwd;
}

void Password::Setnum(int site, uint8_t num)
{
	if (site < SIZE)
	{
		plaintext[site / COL][site % COL] = num;
	}
}

uint8_t Password::Getnum(int site)
{
	if (site < SIZE)
	{
		return plaintext[site / COL][site % COL];
	}
	return 0;
}

Password Password::operator^(const Password &pwd)
{
	Password temp;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			temp.plaintext[i][j] = plaintext[i][j] ^ pwd.plaintext[i][j];
		}
	}
	return temp;
}

Password &Password::operator^=(Password key)
{
	*this = *this ^ key;
	return *this;
}

Password Password::operator*(const Password &pwd)
{
	// 在作用域上的乘法
	Password temp;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			uint8_t num = 0;
			for (int k = 0; k < COL; k++)
			{
				num ^= multiply(this->plaintext[i][k], pwd.plaintext[k][j]);
			}
			temp.plaintext[i][j] = num;
		}
	}
	return temp;
}

Password &Password::operator*=(Password pwd)
{
	*this = *this * pwd;
	return *this;
}

uint8_t Password::multiply(uint8_t a, uint8_t b)
{
	uint8_t temp[8] = {a};
	uint8_t tempmultiply = 0x00;
	for (int i = 1; i < 8; i++)
	{
		temp[i] = XTIME(temp[i - 1]);
	}
	tempmultiply = (b & 0x01) * a;
	for (int i = 1; i <= 7; i++)
	{
		tempmultiply ^= (((b >> i) & 0x01) * temp[i]);
	}
	return tempmultiply;
}

uint8_t Password::XTIME(uint8_t x)
{
	return ((x << 1) ^ ((x & 0x80) ? 0x1b : 0x00));
}

ostream &operator<<(ostream &out, Password pwd)
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (NUMDISPLAY)
			{
				cout << +pwd.plaintext[i][j] << " ";
			}
			else
			{
				cout << pwd.plaintext[i][j] << " ";
			}
		}
		cout << endl;
	}
	return out;
}

ostream &operator<<(ostream &out, AES aes)
{
	cout << aes.Key;
	return out;
}

AES::AES(const Password &key)
{
	this->Key = key;
	RoundNum = 0;
}

Password AES::PasswordInit(CString pwd)
{
	CStringA Tpwd;
	Tpwd = pwd;
	return Password(Tpwd.GetBuffer());
}

CStringA AES::Encryption(CString pwd, const Password &key)
{
	CStringA Tpwd;
	int len = pwd.GetLength();
	Password password;
	uint8_t *buf = NULL;
	while (len >= 0)
	{
		// 分割明文
		password = PasswordInit(pwd.Left(SIZE));
		pwd.Delete(0, SIZE);
		len = len - SIZE;
		// 重置密钥
		Key = key;
		RoundNum = 0;
		// 处理password
		InitRound(password);
		for (int i = 0; i < 9; i++)
		{
			CycleRound(password);
		}
		FinalRound(password);
		buf = password.GetChar();
		Tpwd.Append(CStringA(buf));
	}
	delete[] buf;
	return Tpwd;
}

CString AES::Decryption(const char *pwd, const Password &key)
{
	CString str;
	CStringA Tstr;
	int len = strlen(pwd);
	Password password;
	uint8_t *buf = NULL;
	while (len > 0)
	{
		// 分割密文
		password = Password((char *)pwd);
		len = len - SIZE;
		pwd += SIZE;
		// 重置密钥
		Key = key;
		RoundNum = 0;

		DeInitRound(password);
		for (int i = 0; i < 9; i++)
		{
			Key = key;
			DeCycleRound(password);
		}
		Key = key;
		DeFinalRound(password);
		buf = password.GetChar();
		Tstr.Append(CStringA(buf));
	}
	// 裁剪末尾无用信息
	len = Tstr.GetAt(Tstr.GetLength() - 1);
	Tstr.Delete(Tstr.GetLength() - len, len);
	str = Tstr;
	delete[] buf;
	return str;
}

void AES::SetKey(const Password &key)
{
	this->Key = key;
}

void AES::InitRound(Password &password)
{
	password ^= Key;
}

void AES::CycleRound(Password &password)
{
	SubBytes(password);
	ShiftRows(password);
	MixColumns(password);
	AddRoundKey();
	password ^= Key;
}

void AES::FinalRound(Password &password)
{
	SubBytes(password);
	ShiftRows(password);
	AddRoundKey();
	password ^= Key;
}

void AES::SubBytes(Password &password)
{
	uint8_t temp = 0;
	for (int i = 0; i < SIZE; i++)
	{
		temp = password.Getnum(i);
		password.Setnum(i, SBox[temp]);
	}
}

void AES::ShiftRows(Password &password)
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < i; j++)
		{
			uint8_t temp = password.Getnum(COL * i);
			for (int k = 0; k < COL - 1; k++)
			{
				password.Setnum(COL * i + k, password.Getnum(COL * i + k + 1));
			}
			password.Setnum(COL * i + COL - 1, temp);
		}
	}
}

void AES::MixColumns(Password &password)
{
	password = SMixCol * password;
}

void AES::AddRoundKey()
{
	uint8_t *buf = NULL;
	for (int i = 0; i < COL; i++)
	{
		buf = Key.GetChar();
		CStringA RoundKey = buf;
		RoundKey.Delete(0, ROW);
		Password Temp = Password(RoundKey.GetBuffer());
		uint8_t num = 0;
		if (i == 0)
		{
			Password Tkey = Key;
			// 字循环
			uint8_t Tnum = Tkey.Getnum(COL - 1);
			for (int j = 0; j < ROW - 1; j++)
			{
				Tkey.Setnum(j * COL + COL - 1, Tkey.Getnum((j + 1) * COL + COL - 1));
			}
			Tkey.Setnum((ROW - 1) * COL + COL - 1, Tnum);
			// 字节代换
			for (int j = 0; j < ROW; j++)
			{
				Tnum = SBox[Tkey.Getnum(j * COL + COL - 1)];
				Tkey.Setnum(j * COL + COL - 1, Tnum);
			}
			// 轮常量异或
			Tnum = Tkey.Getnum(COL - 1) ^ Rcon[RoundNum];
			Tkey.Setnum(COL - 1, Tnum);

			for (int j = 0; j < ROW; j++)
			{
				num = Tkey.Getnum(j * COL + COL - 4) ^ (Tkey.Getnum(j * COL + COL - 1));
				Temp.Setnum(j * COL + COL - 1, num);
			}
		}
		else
		{
			for (int j = 0; j < ROW; j++)
			{
				num = Key.Getnum(j * COL + COL - 4) ^ Key.Getnum(j * COL + COL - 1);
				Temp.Setnum(j * COL + COL - 1, num);
			}
		}
		Key = Temp;
	}
	delete[] buf;

	RoundNum++;
}

void AES::DeInitRound(Password &password)
{
	for (int i = 0; i < 10; i++)
	{
		AddRoundKey();
	}
	password ^= Key;
}

void AES::DeCycleRound(Password &password)
{
	DeShiftRows(password);
	DeSubBytes(password);
	DeAddRoundKey();
	password ^= Key;
	DeMixColumns(password);
}

void AES::DeFinalRound(Password &password)
{
	DeShiftRows(password);
	DeSubBytes(password);
	DeAddRoundKey();
	password ^= Key;
}

void AES::DeSubBytes(Password &password)
{
	uint8_t temp = 0;
	for (int i = 0; i < SIZE; i++)
	{
		temp = password.Getnum(i);
		password.Setnum(i, InvSBox[temp]);
	}
}

void AES::DeShiftRows(Password &password)
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < i; j++)
		{
			uint8_t temp = password.Getnum(COL * i + COL - 1);
			for (int k = COL - 1; k > 0; k--)
			{
				password.Setnum(COL * i + k, password.Getnum(COL * i + k - 1));
			}
			password.Setnum(COL * i, temp);
		}
	}
}

void AES::DeMixColumns(Password &password)
{
	password = InvSMixColBox * password;
}

void AES::DeAddRoundKey()
{
	int TroundNum = RoundNum - 1;

	RoundNum = 0;
	for (int i = 0; i < TroundNum; i++)
	{
		AddRoundKey();
	}
}
