
#pragma once
class CGdiPlusPng  
{  
public:  
	Gdiplus::Image* m_pImage;  
public:  
	CGdiPlusPng();
	CGdiPlusPng(LPCWSTR pFile); 
	virtual ~CGdiPlusPng();
	void Empty();
	bool Load(LPCWSTR pFile);
	operator Gdiplus::Image * () const
	{  
		return m_pImage;  
	}  
	Gdiplus::Image *GetPngImage() const;
	int getWidth();
	int getHeight();
};  