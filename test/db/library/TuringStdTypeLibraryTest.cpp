#include "TuringStdTypeLibrary.h"
#include "DB.h"
#include "EdgeType.h"
#include "NodeType.h"
#include "gtest/gtest.h"

namespace db {

class TuringStdTypeLibraryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _db = DB::create();
        _lib = new TuringStdTypeLibrary {_db};
        _lib->load();
    }

    void TearDown() override {
        if (_db) {
            delete _db;
        }

        if (_lib) {
            delete _lib;
        }
    }

    void getNodeType(std::string&& ntName, NodeType** nt) {
        *nt = _db->getNodeType(_db->getString(ntName));
        ASSERT_TRUE(&nt);
    }

    void testPropertyType(std::string&& ptName, ValueType vt, NodeType* nt) {
        PropertyType* pt = nt->getPropertyType(_db->getString(ptName));
        ASSERT_TRUE(pt);
        ASSERT_EQ(pt->getValueType(), vt);
    }

    void testEdgeType(std::string&& etName,
                      std::initializer_list<std::string>&& sourceNames,
                      std::initializer_list<std::string>&& targetNames) {
        EdgeType* et = _db->getEdgeType(_db->getString(etName));
        ASSERT_TRUE(et);

        for (const std::string& sourceName : sourceNames) {
            ASSERT_TRUE(et->hasSourceType(_db->getNodeType(_db->getString(sourceName))));
        }

        for (const std::string& targetName : targetNames) {
            ASSERT_TRUE(et->hasTargetType(_db->getNodeType(_db->getString(targetName))));
        }
    }

private:
    DB* _db {nullptr};
    TuringStdTypeLibrary* _lib {nullptr};
};

TEST_F(TuringStdTypeLibraryTest, LoadDB) {
    {
        NodeType* nt {nullptr};
        getNodeType("Alias", &nt);
        testPropertyType("Name", ValueType::stringType(), nt);
        testPropertyType("Source", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Cell", &nt);
        testPropertyType("Name", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Cellular_Location", &nt);
        testPropertyType("Name", ValueType::stringType(), nt);
        testPropertyType("GO_ID", ValueType::stringType(), nt);
        testPropertyType("Type", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Clinical_Info", &nt);
        testPropertyType("Patients_High", ValueType::decimalType(), nt);
        testPropertyType("Patients_Low", ValueType::decimalType(), nt);
        testPropertyType("Patients_Medium", ValueType::decimalType(), nt);
        testPropertyType("Patients_Not", ValueType::decimalType(), nt);
        testPropertyType("Prognostic_NOT_OK", ValueType::decimalType(), nt);
        testPropertyType("Prognostic_OK", ValueType::decimalType(), nt);
        testPropertyType("UnPrognostic_NOT_OK_HPA", ValueType::decimalType(), nt);
        testPropertyType("UnPrognostic_OK_HPA", ValueType::decimalType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Enrichment_Term", &nt);
        testPropertyType("Type", ValueType::stringType(), nt);
        testPropertyType("Term_ID", ValueType::stringType(), nt);
        testPropertyType("Description", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Gene", &nt);
        testPropertyType("Chromosome", ValueType::intType(), nt);
        testPropertyType("Ensembl_ID", ValueType::stringType(), nt);
        testPropertyType("GeneBank_ID", ValueType::stringType(), nt);
        testPropertyType("GeneCards_ID", ValueType::stringType(), nt);
        testPropertyType("GeneSymbol", ValueType::stringType(), nt);
        testPropertyType("HGNC_ID", ValueType::stringType(), nt);
        testPropertyType("Locus", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Gene_Cluster", &nt);
        testPropertyType("Cluster_ID", ValueType::stringType(), nt);
        testPropertyType("Normalized_TPM", ValueType::decimalType(), nt);
        testPropertyType("Orthology", ValueType::boolType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("GC_Interaction", &nt);
        testPropertyType("COG_1", ValueType::stringType(), nt);
        testPropertyType("COG_2", ValueType::stringType(), nt);
        testPropertyType("COG_Interaction_ID", ValueType::stringType(), nt);
        testPropertyType("Coexpression_Score", ValueType::intType(), nt);
        testPropertyType("Coocurrence_Score", ValueType::intType(), nt);
        testPropertyType("Database_Score", ValueType::intType(), nt);
        testPropertyType("Experimental_Score", ValueType::intType(), nt);
        testPropertyType("Fusion_Score", ValueType::intType(), nt);
        testPropertyType("Neighborhood_Score", ValueType::intType(), nt);
        testPropertyType("Score_String", ValueType::decimalType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("GO_ID", &nt);
        testPropertyType("GO_ID", ValueType::stringType(), nt);
        testPropertyType("Description", ValueType::stringType(), nt);
        testPropertyType("Category", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Homology", &nt);
        testPropertyType("Bitscore", ValueType::decimalType(), nt);
        testPropertyType("End_1", ValueType::intType(), nt);
        testPropertyType("End_2", ValueType::intType(), nt);
        testPropertyType("Protein_1", ValueType::stringType(), nt);
        testPropertyType("Protein_2", ValueType::stringType(), nt);
        testPropertyType("Size", ValueType::intType(), nt);
        testPropertyType("Start_1", ValueType::intType(), nt);
        testPropertyType("Start_2", ValueType::intType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Interaction", &nt);
        //testPropertyType("Annotation", ValueType::listType(), nt);
        //testPropertyType("CheckSum", ValueType::listType(), nt);
        //testPropertyType("Created_At", ValueType::dateType(), nt);
        testPropertyType("Coexpression_Score", ValueType::intType(), nt);
        testPropertyType("Coocurrence_Score", ValueType::intType(), nt);
        testPropertyType("Database_Score", ValueType::intType(), nt);
        testPropertyType("Experimental_Score", ValueType::intType(), nt);
        //testPropertyType("Expansion_Method", ValueType::listType(), nt);
        testPropertyType("Fusion_Score", ValueType::intType(), nt);
        testPropertyType("Homology_Score", ValueType::intType(), nt);
        testPropertyType("Interaction_Detection_Method", ValueType::stringType(), nt);
        testPropertyType("Negative", ValueType::boolType(), nt);
        testPropertyType("Neighborhood_Score", ValueType::intType(), nt);
        //testPropertyType("Parameters", ValueType::listType(), nt);
        testPropertyType("Physical", ValueType::boolType(), nt);
        testPropertyType("Protein_1", ValueType::stringType(), nt);
        testPropertyType("Protein_2", ValueType::stringType(), nt);
        testPropertyType("Score_Authors", ValueType::decimalType(), nt);
        testPropertyType("Score_IntAct", ValueType::decimalType(), nt);
        testPropertyType("Score_String", ValueType::decimalType(), nt);
        testPropertyType("Score_Mentha", ValueType::decimalType(), nt);
        testPropertyType("Source", ValueType::stringType(), nt);
        testPropertyType("Stoichiometry_1", ValueType::intType(), nt);
        testPropertyType("Stoichiometry_2", ValueType::intType(), nt);
        testPropertyType("Type", ValueType::stringType(), nt);
        //testPropertyType("Updated_At", ValueType::dateType(), nt);
        //testPropertyType("xRef", ValueType::listType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Metabolite", &nt);
        testPropertyType("Acceptor_Count", ValueType::stringType(), nt);
        //testPropertyType("Alias", ValueType::listType(), nt);
        //testPropertyType("Alt_IDs", ValueType::listType(), nt);
        testPropertyType("AVG_Weight", ValueType::intType(), nt);
        testPropertyType("BiGG_ID", ValueType::stringType(), nt);
        testPropertyType("BioCyc_ID", ValueType::stringType(), nt);
        testPropertyType("Bioavailability", ValueType::stringType(), nt);
        testPropertyType("Boiling_Point", ValueType::stringType(), nt);
        testPropertyType("CAS_Number", ValueType::stringType(), nt);
        testPropertyType("ChEBI_ID", ValueType::stringType(), nt);
        testPropertyType("ChemSpider_ID", ValueType::stringType(), nt);
        //testPropertyType("Created_At", ValueType::dateType(), nt);
        testPropertyType("Description", ValueType::stringType(), nt);
        testPropertyType("DrugBank_ID", ValueType::stringType(), nt);
        testPropertyType("Donor_Count", ValueType::stringType(), nt);
        testPropertyType("FBOnto_ID", ValueType::intType(), nt);
        //testPropertyType("FooDB_ID", ValueType::listType(), nt);
        testPropertyType("Formal_Charge", ValueType::stringType(), nt);
        //testPropertyType("Formula", ValueType::listType(), nt);
        testPropertyType("GeneBank_ID", ValueType::stringType(), nt);
        testPropertyType("Ghose_Filter", ValueType::boolType(), nt);
        testPropertyType("HMDB_ID", ValueType::stringType(), nt);
        testPropertyType("HMDB_Status", ValueType::stringType(), nt);
        testPropertyType("InChI", ValueType::stringType(), nt);
        testPropertyType("InChIKey", ValueType::stringType(), nt);
        testPropertyType("IUPAC_Name", ValueType::stringType(), nt);
        testPropertyType("KEGG_ID", ValueType::stringType(), nt);
        testPropertyType("KNApSAcK_ID", ValueType::stringType(), nt);
        testPropertyType("logP", ValueType::intType(), nt);
        testPropertyType("logS", ValueType::intType(), nt);
        testPropertyType("MDDR_Like_Rule", ValueType::boolType(), nt);
        testPropertyType("Melting_Point", ValueType::intType(), nt);
        testPropertyType("METLIN_ID", ValueType::intType(), nt);
        testPropertyType("Monoisotropic_Weight", ValueType::intType(), nt);
        testPropertyType("Number_of_Rings", ValueType::intType(), nt);
        testPropertyType("Phenol_Explorer_ID", ValueType::stringType(), nt);
        testPropertyType("Physiological_Charge", ValueType::stringType(), nt);
        testPropertyType("Polar_Surface_Area", ValueType::stringType(), nt);
        testPropertyType("PDB_ID", ValueType::stringType(), nt);
        testPropertyType("pKa_Strongest_Basic", ValueType::intType(), nt);
        testPropertyType("pKa_Strongest_Acidic", ValueType::intType(), nt);
        testPropertyType("Polarizability", ValueType::intType(), nt);
        testPropertyType("PubChem_ID", ValueType::stringType(), nt);
        testPropertyType("Refractivity", ValueType::intType(), nt);
        testPropertyType("Rotatable_Bond_Count", ValueType::intType(), nt);
        testPropertyType("Rule_of_Five", ValueType::boolType(), nt);
        testPropertyType("SMILES", ValueType::intType(), nt);
        testPropertyType("Solubility", ValueType::intType(), nt);
        //testPropertyType("Updated_At", ValueType::dateType(), nt);
        testPropertyType("Veber_Rule", ValueType::boolType(), nt);
        testPropertyType("VMH_ID", ValueType::intType(), nt);
        testPropertyType("Water_Solubility", ValueType::stringType(), nt);
        testPropertyType("WikiPedia_ID", ValueType::stringType(), nt);
        //testPropertyType("xRef", ValueType::listType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Mutation", &nt);
        testPropertyType("beta_value", ValueType::decimalType(), nt);
        testPropertyType("Chromosome", ValueType::stringType(), nt);
        testPropertyType("Mutated_Sequence", ValueType::stringType(), nt);
        testPropertyType("Mutation_ID", ValueType::stringType(), nt);
        testPropertyType("p_value", ValueType::decimalType(), nt);
        testPropertyType("Position", ValueType::intType(), nt);
        testPropertyType("Reference_Sequence", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Measurement", &nt);
        testPropertyType("Age", ValueType::stringType(), nt);
        testPropertyType("Concentration_Value", ValueType::decimalType(), nt);
        testPropertyType("Concentration_Units", ValueType::stringType(), nt);
        testPropertyType("Condition", ValueType::decimalType(), nt);
        testPropertyType("Normal", ValueType::intType(), nt);
        testPropertyType("Sex", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Organ", &nt);
        testPropertyType("Name", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Orthology", &nt);
        //testPropertyType("Annotation", ValueType::listType(), nt);
        testPropertyType("End", ValueType::intType(), nt);
        testPropertyType("Protein_1", ValueType::stringType(), nt);
        testPropertyType("Start", ValueType::intType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Pathology", &nt);
        testPropertyType("Name", ValueType::stringType(), nt);
        testPropertyType("OMIM_ID", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Pathway", &nt);
        testPropertyType("Kegg_Map_ID", ValueType::stringType(), nt);
        testPropertyType("Name", ValueType::stringType(), nt);
        testPropertyType("SMPDB_ID", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("PDB_ID", &nt);
        testPropertyType("PDB_ID", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("PFam", &nt);
        testPropertyType("Description", ValueType::stringType(), nt);
        testPropertyType("PFAM_ID", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Protein", &nt);
        //testPropertyType("Alias", ValueType::listType(), nt);
        //testPropertyType("Alt_IDs", ValueType::listType(), nt);
        //testPropertyType("Annotation", ValueType::listType(), nt);
        testPropertyType("Biological_Role", ValueType::stringType(), nt);
        testPropertyType("CheckSum", ValueType::stringType(), nt);
        //testPropertyType("Created_At", ValueType::dateType(), nt);
        testPropertyType("Description", ValueType::stringType(), nt);
        testPropertyType("Ensembl_ID", ValueType::stringType(), nt);
        testPropertyType("Experimental_Role", ValueType::stringType(), nt);
        //testPropertyType("Features", ValueType::listType(), nt);
        testPropertyType("GeneBank_ID", ValueType::stringType(), nt);
        testPropertyType("General_Function", ValueType::stringType(), nt);
        testPropertyType("HMDB_ID", ValueType::stringType(), nt);
        testPropertyType("Identification_Method_Participant", ValueType::stringType(), nt);
        testPropertyType("Length", ValueType::intType(), nt);
        testPropertyType("Molecular_Weight", ValueType::intType(), nt);
        testPropertyType("Protein_ID", ValueType::stringType(), nt);
        //testPropertyType("Signalling_Regions", ValueType::listType(), nt);
        testPropertyType("Size", ValueType::intType(), nt);
        testPropertyType("Specific_Function", ValueType::stringType(), nt);
        testPropertyType("Theoretical_PI", ValueType::stringType(), nt);
        //testPropertyType("Transmembrane_Regions", ValueType::listType(), nt);
        testPropertyType("Type", ValueType::stringType(), nt);
        testPropertyType("UniProt_ID", ValueType::stringType(), nt);
        testPropertyType("UniProt_Name", ValueType::stringType(), nt);
        //testPropertyType("Updated_At", ValueType::dateType(), nt);
        //testPropertyType("xRef", ValueType::listType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Protein_Cluster", &nt);
        testPropertyType("Cluster_ID", ValueType::stringType(), nt);
        testPropertyType("Description", ValueType::stringType(), nt);
        testPropertyType("Size", ValueType::intType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Publication", &nt);
        //testPropertyType("Authors", ValueType::listType(), nt);
        //testPropertyType("Created_At", ValueType::dateType(), nt);
        //testPropertyType("Date", ValueType::dateType(), nt);
        testPropertyType("doi", ValueType::stringType(), nt);
        testPropertyType("IMEx_ID", ValueType::stringType(), nt);
        testPropertyType("Journal", ValueType::stringType(), nt);
        testPropertyType("MINT_ID", ValueType::stringType(), nt);
        testPropertyType("Number", ValueType::stringType(), nt);
        testPropertyType("Title", ValueType::stringType(), nt);
        testPropertyType("PubMed_ID", ValueType::stringType(), nt);
        //testPropertyType("Updated_At", ValueType::dateType(), nt);
        testPropertyType("Volume", ValueType::intType(), nt);
        testPropertyType("Year", ValueType::intType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Sequence", &nt);
        testPropertyType("Type", ValueType::stringType(), nt);
        testPropertyType("Format", ValueType::stringType(), nt);
        testPropertyType("Raw_Sequence", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Taxonomy", &nt);
        testPropertyType("NCBI_Tax_ID", ValueType::stringType(), nt);
        testPropertyType("Name", ValueType::stringType(), nt);
        testPropertyType("String_Type", ValueType::stringType(), nt);
        testPropertyType("Type", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Tissue", &nt);
        testPropertyType("Name", ValueType::stringType(), nt);
    }
    {
        NodeType* nt {nullptr};
        getNodeType("Transcript", &nt);
        testPropertyType("CAGE_TPM", ValueType::decimalType(), nt);
        testPropertyType("Cell_Cycle_Dependency", ValueType::boolType(), nt);
        testPropertyType("Certainty", ValueType::stringType(), nt);
        testPropertyType("Count_TPM", ValueType::intType(), nt);
        testPropertyType("Length", ValueType::intType(), nt);
        testPropertyType("Level", ValueType::stringType(), nt);
        testPropertyType("Max_TPM", ValueType::decimalType(), nt);
        testPropertyType("Mean_TPM", ValueType::decimalType(), nt);
        testPropertyType("Min_TPM", ValueType::decimalType(), nt);
        testPropertyType("Normalized_CAGE_TPM", ValueType::decimalType(), nt);
        testPropertyType("Normalized_TPM", ValueType::decimalType(), nt);
        testPropertyType("Protein_TPM", ValueType::decimalType(), nt);
        testPropertyType("Q1_TPM", ValueType::decimalType(), nt);
        testPropertyType("Q2_TPM", ValueType::decimalType(), nt);
        testPropertyType("Q3_TPM", ValueType::decimalType(), nt);
        testPropertyType("Read_Count", ValueType::decimalType(), nt);
        testPropertyType("Reliability", ValueType::stringType(), nt);
        testPropertyType("Scaled_CAGE_TPM", ValueType::decimalType(), nt);
        testPropertyType("Single_Cell_Variation_Intensity", ValueType::boolType(), nt);
        testPropertyType("Single_Cell_Variation_Spatial", ValueType::boolType(), nt);
        testPropertyType("STD_TPM", ValueType::decimalType(), nt);
    }
    testEdgeType("Aliased_As",
                 {"Protein", "Taxonomy", "Metabolite"},
                 {"Alias"});
    testEdgeType("Associated_with_Pathology",
                 {"Gene"},
                 {"Pathology"});
    testEdgeType("Cited_In",
                 {"Interaction"},
                 {"Publication"});
    testEdgeType("Encoded_By",
                 {"Protein", "Metabolite"},
                 {"Gene"});
    testEdgeType("Expressed_In_Cell",
                 {"Gene", "Transcript", "Mutation"},
                 {"Cell"});
    testEdgeType("Expressed_In_Tissue",
                 {"Gene", "Transcript", "Gene_Cluster"},
                 {"Tissue"});
    testEdgeType("GC_Interacts_with_GC",
                 {"Gene_Cluster"},
                 {"GC_Interaction"});
    testEdgeType("Located_Inside_Cell",
                 {"Protein", "Metabolite"},
                 {"Cellular_Location"});
    testEdgeType("Measured_As",
                 {"Protein", "Metabolite"},
                 {"Measurement"});
    testEdgeType("Measured_Inside",
                 {"Measurement"},
                 {"Tissue"});
    testEdgeType("Member_of_Interaction",
                 {"Protein", "Metabolite"},
                 {"Interaction"});
    testEdgeType("Part_of_Cluster",
                 {"Protein", "Protein_Cluster"},
                 {"Protein_Cluster"});
    testEdgeType("Part_of_Homology",
                 {"Protein"},
                 {"Homology"});
    testEdgeType("Part_of_Organ",
                 {"Tissue"},
                 {"Organ"});
    testEdgeType("Part_of_Orthology",
                 {"Protein", "Orthology"},
                 {"Orthology", "Gene_Cluster"});
    testEdgeType("Part_of_Pathway",
                 {"Protein", "Metabolite"},
                 {"Pathway"});
    testEdgeType("Part_of_Pfam",
                 {"Protein"},
                 {"PFam"});
    testEdgeType("Part_of_Taxonomy",
                 {"Protein", "Protein_Cluster", "Gene_Cluster",
                 "Interaction", "Transcript", "Homology", "Metabolite"},
                 {"Taxonomy"});
    testEdgeType("Part_of_Tissue",
                 {"Tissue"},
                 {"Tissue"});
    testEdgeType("Related_Clinical_Info",
                 {"Gene", "Pathology"},
                 {"Clinical_Info"});
    testEdgeType("Related_with_GO_ID",
                 {"Protein"},
                 {"GO_ID"});
    testEdgeType("Related_with_Term",
                 {"Protein", "Metabolite"},
                 {"Enrichment_Term"});
    testEdgeType("Sequenced_As",
                 {"Protein", "Gene"},
                 {"Sequence"});
    testEdgeType("Transcribed_As",
                 {"Gene"},
                 {"Transcript"});
}
}
