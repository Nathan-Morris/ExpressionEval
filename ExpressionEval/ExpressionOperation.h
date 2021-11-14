#include <iostream>
#include <map>
#pragma once

typedef float(*BinaryOperationCallback)(float, float);
typedef float(*UnaryOperationCallback)(float);

enum class OperationIdOpType : unsigned short {
	UNARY = 0,
	BINARY = 1
};

class OperationId {
private:
	static unsigned short LAST_ID;

private:
	unsigned short mId : 12;		// [0, 4095]
	unsigned short mPrecedent : 3;	// [0, 7]
	unsigned short mOpType : 1;		// [0, 1] -> [unary, binary]

public:
	OperationId();

	OperationId(unsigned short type);
	OperationId(unsigned short type, unsigned short precedent);
	OperationId(unsigned short type, unsigned short opIdBase, unsigned short precedent);

	unsigned short id() const;
	unsigned short precedent() const;
	OperationIdOpType type() const;

	unsigned short id(unsigned short value);
	unsigned short precedent(unsigned short value);
	OperationIdOpType type(unsigned short value);

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
	void* mOpCallback = NULL;
	OperationId mOpId;

public:
	ExpressionOperationInfo(const char* token, OperationId id, UnaryOperationCallback cb);
	ExpressionOperationInfo(const char* token, OperationId id, BinaryOperationCallback cb);
	ExpressionOperationInfo(const char* token);
	ExpressionOperationInfo();

public:
	const OperationId& opId() const;
	const char* token() const;
	unsigned char tokenLen() const;

	UnaryOperationCallback unaryCallback() const;
	BinaryOperationCallback binaryCallback() const;

// static members
private:
	static std::map<OperationId, ExpressionOperationInfo> DECLARED_OPERATIONS;

public:
	static bool findOperation(const char* token, const ExpressionOperationInfo** storePtr);
	static bool findOperation(OperationId opId, const ExpressionOperationInfo** storePtr);

	static void declareOperations(const std::initializer_list<ExpressionOperationInfo>& operations);
};

