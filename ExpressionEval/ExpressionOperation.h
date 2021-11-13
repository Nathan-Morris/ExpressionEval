#include <iostream>
#include <map>
#pragma once

class OperationId {
private:
	unsigned short mId : 12;		// [0, 4095]
	unsigned short mPrecedent : 4;	// [0, 15]

public:
	OperationId();

	// precedent range of [0, 15]
	OperationId(unsigned short opIdBase, unsigned short precedent);

	unsigned short id() const;
	unsigned short precedent() const;

	unsigned short id(unsigned short value);
	unsigned short precedent(unsigned short value);

public:
	friend bool operator<(const OperationId& opIdRefL, const OperationId& opIdRefR);
};

//
//
//

class ExpressionOperationInfo {
protected:
	const char* mToken = NULL;
	unsigned char mTokenLen = 0;
	OperationId mOpId;

	ExpressionOperationInfo(const char* token, OperationId id);
	ExpressionOperationInfo(const char* token);
	ExpressionOperationInfo();

public:
	const OperationId& opId() const;
	const char* token() const;
	unsigned char tokenLen() const;
};

//
//
//

class BinaryOperationInfo : public ExpressionOperationInfo {
public:
	static std::map<OperationId, BinaryOperationInfo> DECLARED_OPERATIONS;
	static unsigned short DECLARED_LAST_ID;

public:
	BinaryOperationInfo();
	BinaryOperationInfo(const char* token, unsigned short precedent);

private:
	static void declareOperations();

public:

	template<typename CharArrType, typename UshortType, typename ...Rest>
	static void declareOperations(
		const CharArrType& token, 
		const UshortType& precedent, 
		const Rest&... rest
	) {
		BinaryOperationInfo info(token, precedent);
		BinaryOperationInfo::DECLARED_OPERATIONS[info.opId()] = info;
		BinaryOperationInfo::declareOperations(rest...);
	}

	static bool findOperation(const char* cstr, const void** ptrStore);
	static bool findOperation(OperationId id, const void** ptrStore);
};

//
//
//

class UnaryOperationInfo : public ExpressionOperationInfo {
public:
	static std::map<OperationId, UnaryOperationInfo> DECLARED_OPERATIONS;
	static unsigned short DECLARED_LAST_ID;

public:
	UnaryOperationInfo(const char* token);
	UnaryOperationInfo();

private:
	static void declareOperations();

public:

	template<typename CharArrType, typename ...Rest>
	static void declareOperations(
		const CharArrType& token,
		const Rest&... rest
	) {
		UnaryOperationInfo info(token);
		UnaryOperationInfo::DECLARED_OPERATIONS[info.opId()] = info;
		UnaryOperationInfo::declareOperations(rest...);
	}

	static bool findOperation(const char* cstr, const void** ptrStore);
	static bool findOperation(OperationId id, const void** ptrStore);
};
