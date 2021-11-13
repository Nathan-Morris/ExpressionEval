#include "ExpressionOperation.h"

OperationId::OperationId() : OperationId(0, 0) { }

OperationId::OperationId(unsigned short opIdBase, unsigned short precedent) {
	if (precedent > 0xF) {
		precedent = 0xF;
	}
	this->mId = opIdBase;
	this->mPrecedent = precedent;
}

unsigned short OperationId::id() const {
	return this->mId;
}

unsigned short OperationId::precedent() const {
	return this->mPrecedent;
}

unsigned short OperationId::id(unsigned short value) {
	return (this->mId = (value > 0xFFF) ? 0xFFF : value);
}

unsigned short OperationId::precedent(unsigned short value) {
	return (this->mPrecedent = (value > 0xF) ? 0xF : value);
}

bool operator<(const OperationId& opIdRefL, const OperationId& opIdRefR) {
	return opIdRefL.mId < opIdRefR.mId;
}

//
//
//

ExpressionOperationInfo::ExpressionOperationInfo(const char* token, OperationId id)
	: mToken(token), mTokenLen(strlen(token)), mOpId(id) { }

ExpressionOperationInfo::ExpressionOperationInfo(const char* token)
	: mToken(token), mTokenLen(strlen(token)) { }

ExpressionOperationInfo::ExpressionOperationInfo() { }

const OperationId& ExpressionOperationInfo::opId() const {
	return this->mOpId;
}

const char* ExpressionOperationInfo::token() const {
	return this->mToken;
}

unsigned char ExpressionOperationInfo::tokenLen() const {
	return this->mTokenLen;
}

//
//
//

std::map<OperationId, BinaryOperationInfo> BinaryOperationInfo::DECLARED_OPERATIONS = { };
unsigned short BinaryOperationInfo::DECLARED_LAST_ID = 0;

BinaryOperationInfo::BinaryOperationInfo() {

}

BinaryOperationInfo::BinaryOperationInfo(const char* token, unsigned short precedent)
	: ExpressionOperationInfo(token) {
	this->mOpId = OperationId(DECLARED_LAST_ID++, precedent);
}

void BinaryOperationInfo::declareOperations() {
	return;
}

bool BinaryOperationInfo::findOperation(const char* cstr, const void** ptrStore) {
	for (const auto& declOp : DECLARED_OPERATIONS) {
		if (!strncmp(cstr, declOp.second.mToken, declOp.second.mTokenLen)) {
			*ptrStore = (void*)&declOp.second;
			return true;
		}
	}
	return false;
}

bool BinaryOperationInfo::findOperation(OperationId id, const void** ptrStore) {
	auto iter = DECLARED_OPERATIONS.find(id);
	if (iter != DECLARED_OPERATIONS.end()) {
		*ptrStore = (void*)&iter->second;
		return true;
	} 
	return false;
}

//
//
//

std::map<OperationId, UnaryOperationInfo> UnaryOperationInfo::DECLARED_OPERATIONS = { };
unsigned short UnaryOperationInfo::DECLARED_LAST_ID = 0;

UnaryOperationInfo::UnaryOperationInfo(const char* token) 
	: ExpressionOperationInfo(token, OperationId(DECLARED_LAST_ID++, 0xF)) {
}

UnaryOperationInfo::UnaryOperationInfo() {

}

void UnaryOperationInfo::declareOperations() {
	return;
}

bool UnaryOperationInfo::findOperation(const char* cstr, const void** ptrStore) {
	for (const auto& declOp : DECLARED_OPERATIONS) {
		if (!strncmp(cstr, declOp.second.mToken, declOp.second.mTokenLen)) {
			*ptrStore = (void*)&declOp.second;
			return true;
		}
	}
	return false;
}

bool UnaryOperationInfo::findOperation(OperationId id, const void** ptrStore) {
	auto iter = DECLARED_OPERATIONS.find(id);
	if (iter != DECLARED_OPERATIONS.end()) {
		*ptrStore = (void*)&iter->second;
		return true;
	}
	return false;
}