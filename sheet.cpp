#include "sheet.h"

#include "cell.h"
#include "common.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

using namespace std::literals;

Sheet::~Sheet() {}

void Sheet::SetCell(Position pos, std::string text) {
    if (pos.IsValid()) sheet_[pos].Set(std::move(text)); 
    else throw InvalidPositionException("Invalid position");
}

const CellInterface* Sheet::GetCell(Position pos) const {
    if (!pos.IsValid()) throw InvalidPositionException("Invalid position");
    if (sheet_.count(pos) == 0) return nullptr;
    if (sheet_.at(pos).GetText().empty()) return nullptr;
    return &sheet_.at(pos);
}
CellInterface* Sheet::GetCell(Position pos) {
    if (!pos.IsValid()) throw InvalidPositionException("Invalid position");
    if (sheet_.count(pos) == 0) return nullptr;
    if (sheet_[pos].GetText().empty()) return nullptr;
    return &sheet_[pos];
}

void Sheet::ClearCell(Position pos) {
    if (!pos.IsValid()) throw InvalidPositionException("Invalid position");
    sheet_[pos].Clear();
}

Size Sheet::GetPrintableSize() const {
    Size size{ 0, 0 };

    for (const auto& [pos, cell] : sheet_) {
        if (GetCell(pos) == nullptr) continue;
        if (pos.row >= size.rows) size.rows = pos.row + 1;
        if (pos.col >= size.cols) size.cols = pos.col + 1;
    }

    return size;
}

inline std::ostream& operator<<(std::ostream& output, const CellInterface::Value& value) {
    std::visit(
        [&](const auto& x) {
            output << x;
        },
        value);
    return output;
}

void Sheet::PrintValues(std::ostream& output) const {
    Size size = GetPrintableSize();
    for (int row = 0; row < size.rows; row++) {
        bool is_first = true;
        for (int col = 0; col < size.cols; col++) {
            Position temp_pos{ row,col };
            const CellInterface* cell = GetCell(std::move(temp_pos));

            if (is_first) is_first = false;
            else output << "\t";

            if (cell != nullptr) output << cell->GetValue();
        }
        output << "\n";
    }
}
void Sheet::PrintTexts(std::ostream& output) const {
    Size size = GetPrintableSize();
    for (int row = 0; row < size.rows; row++) {
        bool is_first = true;
        for (int col = 0; col < size.cols; col++) {
            Position temp_pos{ row,col };
            const CellInterface* cell = GetCell(std::move(temp_pos));

            if (is_first) is_first = false;
            else output << "\t";

            if (cell != nullptr) output << cell->GetText();
        }
        output << "\n";
    }
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}
