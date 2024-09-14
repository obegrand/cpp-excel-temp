#include "cell.h"

#include <cassert>
#include <iostream>
#include <string>
#include <optional>

// Реализуйте следующие методы
Cell::Cell() {
	impl_ = std::make_unique<EmptyImpl>();
}

Cell::~Cell() {
	// тут вроде ненадо ничего делать?
}

void Cell::Set(std::string text) {
	if (text.size() == 0) {
		impl_ = std::make_unique<EmptyImpl>();
	}
	else if (text.size() > 1 && text[0] == '=') {
		impl_ = std::make_unique<FormulaImpl>(std::move(text));
	}
	else {
		impl_ = std::make_unique<TextImpl>(std::move(text));
	}
}

void Cell::Clear() {
	impl_ = std::make_unique<EmptyImpl>();
}

Cell::Value Cell::GetValue() const {
	return impl_->GetValue();
}

std::string Cell::GetText() const {
	return impl_->GetText();
}

class Impl {
public:
	virtual Value GetValue() const = 0;
	virtual std::string GetText() const = 0;
protected:
	Value value_;
	std::string text_;
};

class EmptyImpl : public Impl {
public:
	EmptyImpl() {
		value_ = "";
		text_ = "";
	}

	Value GetValue() const override {
		return value_;
	}

	std::string GetText() const override {
		return text_;
	}
};

class TextImpl : public Impl {
public:
	TextImpl(std::string text) {
		text_ = text;
		if (text[0] == '\'') {
			text = text.substr(1);
		}
		value_ = text;
	}

	Value GetValue() const override {
		return value_;
	}

	std::string GetText() const override {
		return text_;
	}
};

class FormulaImpl : public Impl {
public:
	FormulaImpl(std::string expression) {
		expression = expression.substr(1);
		formula_ = ParseFormula(expression);
		text_ = "=" + formula_->GetExpression();
		value_ = expression;
	}

	CellInterface::Value GetValue() const override {
		FormulaInterface::Value temp_value = formula_->Evaluate();
		if (std::holds_alternative<double>(temp_value)) {
			return std::get<double>(temp_value);
		}
		return std::get<FormulaError>(temp_value);
	}

	std::string GetText() const override {
		return text_;
	}
private:
	std::unique_ptr<FormulaInterface> formula_;
};