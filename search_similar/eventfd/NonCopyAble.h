 ///
 /// @file    Noncopyable.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-20 17:11:30
 ///

#ifndef __WD_NONCOPYABLE_H__
#define __WD_NONCOPYABLE_H__

class NonCopyAble
{
protected:
	NonCopyAble() {}
	~NonCopyAble() {}

	NonCopyAble(const NonCopyAble &) = delete;
	NonCopyAble & operator=(const NonCopyAble &) = delete;
};

#endif
