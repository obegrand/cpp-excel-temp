#pragma once

#include "common.h"
#include "formula.h"

class Cell : public CellInterface {
public:
	Cell();
	~Cell();

	void Set(std::string text);
	void Clear();

	Value GetValue() const override;
	std::string GetText() const override;

private:
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

		Value GetValue() const override {
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

	std::unique_ptr<Impl> impl_;
};