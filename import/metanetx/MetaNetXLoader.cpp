#include "MetaNetXLoader.h"

#include <iostream>

#include "RowReader.h"
#include "StringBuffer.h"

#include "DB.h"
#include "Chemical.h"
#include "ChemicalFormula.h"

using namespace db;
using namespace formats;

MetaNetXConfig::MetaNetXConfig(const std::string& metanetxPath)
    : _metanetxPath(metanetxPath)
{
    _chemPropPath = metanetxPath + "/chem_prop.tsv";
}

MetaNetXLoader::MetaNetXLoader(const MetaNetXConfig& config, db::DB* db)
    : _config(config), _db(db)
{
}

bool MetaNetXLoader::load() {
    const bool res = loadChemProp();
    if (!res) {
        return false;
    }

    return true;
}

bool MetaNetXLoader::loadChemProp() {
    StringBuffer* chemPropBuffer = StringBuffer::readFromFile(_config.getChemPropPath());
    if (!chemPropBuffer) {
        return false;
    }

    formats::TSVRowReader tsvReader(chemPropBuffer);

    const size_t ChemPropMinRowSize = 3;

    const auto end = tsvReader.end();
    size_t rowIndex = 0;
    for (auto it = tsvReader.begin(); it != end; ++it, rowIndex++) {
        const auto& row = *it;
        if (rowIndex <= 1) {
            continue;
        }

        const auto& fields = row.fields();
        const std::size_t fieldsNum = fields.size();
        if (fieldsNum < ChemPropMinRowSize) {
            std::cerr << "Less than 3 fields at row index " << rowIndex << '\n';
            for (const auto& field : fields) {
                std::cerr << "Field: '" << field.getString() << "'\n";
            }
            continue;
        }

        const Field& nameField = fields[1];
        const std::string name = nameField.getString();
        if (name.empty()) {
            std::cerr << "Name empty at row index " << rowIndex << '\n';
            continue;
        }

        Chemical* chemical = Chemical::create(_db);
        chemical->setName(name);

        if (fieldsNum >= 4) {
            const Field& formulaField = fields[3];
            const std::string formulaStr = formulaField.getString();
            if (!formulaStr.empty()) {
                ChemicalFormula* formula = ChemicalFormula::create(_db, formulaStr);
                chemical->setFormula(formula);
            }
        }

        if (fieldsNum >= 5) {
            const Field& chargeField = fields[4];
            const std::string chargeStr = chargeField.getString();
            if (!chargeStr.empty()) {
                const double charge = std::stod(chargeStr);
                chemical->setCharge(charge);
            }
        }

        if (fieldsNum >= 6) {
            const Field& massField = fields[5];
            const std::string massStr = massField.getString();
            if (!massStr.empty()) {
                const double mass = std::stod(massStr);
                chemical->setMass(mass);
            }
        }
    }
    
    return true;
}
