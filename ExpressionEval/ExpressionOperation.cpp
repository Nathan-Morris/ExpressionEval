#include "ExpressionOperation.h"

unsigned short OperationId::LAST_ID = 0;

OperationId::OperationId() : OperationId(0, 0, 0) { }

OperationId::OperationId(unsigned short type) : OperationId(type, 0x7) { }

OperationId::OperationId(unsigned short type, unsigned short precedent) : OperationId(type, LAST_ID++, precedent) { }

OperationId::OperationId(unsigned short type, unsigned short opIdBase, unsigned short precedent) {
	this->id(opIdBase);
	this->precedent(precedent);
	this->type(type);
}

unsigned short OperationId::id() const {
	return this->mId;
}

unsigned short OperationId::precedent() const {
	return this->mPrecedent;
}

unsigned short OperationId::type() const {
	return this->mOpType;
}

unsigned short OperationId::id(unsigned short value) {
	return (this->mId = (value > 0xFFF) ? 0xFFF : value);
}

unsigned short OperationId::precedent(unsigned short value) {
	return (this->mPrecedent = (value > 0x7) ? 0x7 : value);
}

unsigned short OperationId::type(unsigned short value) {
	return (this->mOpType = (bool)value);
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

// static members

std::map<OperationId, ExpressionOperationInfo> ExpressionOperationInfo::DECLARED_OPERATIONS = { };

bool ExpressionOperationInfo::findOperation(const char* token, const ExpressionOperationInfo** storePtr) {
	for (const auto& opPair : DECLARED_OPERATIONS) {
		if (!strncmp(token, opPair.second.mToken, opPair.second.mTokenLen)) {
			*storePtr = &opPair.second;
			return true;
		}
	}
	return false;
}

bool ExpressionOperationInfo::findOperation(OperationId opId, const ExpressionOperationInfo** storePtr) {
	const auto iter = DECLARED_OPERATIONS.find(opId);

	if (iter == DECLARED_OPERATIONS.end()) {
		return false;
	}

	*storePtr = &iter->second;
	return true;
}

void ExpressionOperationInfo::declareOperations(const std::initializer_list<ExpressionOperationInfo>& operations) {
	for (const auto& operationInfo : operations) {
		DECLARED_OPERATIONS[operationInfo.opId()] = operationInfo;
	}
}