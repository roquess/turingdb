#include "TuringStdTypeLibrary.h"
#include "Writeback.h"

#include <unordered_map>

namespace db {

TuringStdTypeLibrary::TuringStdTypeLibrary(DB* db)
    : TypeLibrary(db)
{
}

bool TuringStdTypeLibrary::load() {
    std::unordered_map<const char*, NodeType*> nts;

    nts["Alias"] = createNodeType("Alias", {
        {"Name",   ValueType::stringType()},
        {"Source", ValueType::stringType()},
    });

    nts["Cell"] = createNodeType("Cell", {
        {"Name", ValueType::stringType()},
    });

    nts["Cellular_Location"] = createNodeType("Cellular_Location", {
        {"Name",  ValueType::stringType()},
        {"GO_ID", ValueType::stringType()},
        {"Type",  ValueType::stringType()},
    });

    nts["Clinical_Info"] = createNodeType("Clinical_Info", {
        {"Patients_High",           ValueType::decimalType()},
        {"Patients_Low",            ValueType::decimalType()},
        {"Patients_Medium",         ValueType::decimalType()},
        {"Patients_Not",            ValueType::decimalType()},
        {"Prognostic_NOT_OK",       ValueType::decimalType()},
        {"Prognostic_OK",           ValueType::decimalType()},
        {"UnPrognostic_NOT_OK_HPA", ValueType::decimalType()},
        {"UnPrognostic_OK_HPA",     ValueType::decimalType()},
    });

    nts["Enrichment_Term"] = createNodeType("Enrichment_Term", {
        {"Type",        ValueType::stringType()},
        {"Term_ID",     ValueType::stringType()},
        {"Description", ValueType::stringType()},
    });

    nts["Gene"] = createNodeType("Gene", {
        {"Chromosome",   ValueType::intType()   },
        {"Ensembl_ID",   ValueType::stringType()},
        {"GeneBank_ID",  ValueType::stringType()},
        {"GeneCards_ID", ValueType::stringType()},
        {"GeneSymbol",   ValueType::stringType()},
        {"HGNC_ID",      ValueType::stringType()},
        {"Locus",        ValueType::stringType()},
    });

    nts["Gene_Cluster"] = createNodeType("Gene_Cluster", {
        {"Cluster_ID",     ValueType::stringType() },
        {"Normalized_TPM", ValueType::decimalType()},
        {"Orthology",      ValueType::boolType()   },
    });

    nts["GC_Interaction"] = createNodeType("GC_Interaction", {
        {"COG_1",              ValueType::stringType() },
        {"COG_2",              ValueType::stringType() },
        {"COG_Interaction_ID", ValueType::stringType() },
        {"Coexpression_Score", ValueType::intType()    },
        {"Coocurrence_Score",  ValueType::intType()    },
        {"Database_Score",     ValueType::intType()    },
        {"Experimental_Score", ValueType::intType()    },
        {"Fusion_Score",       ValueType::intType()    },
        {"Neighborhood_Score", ValueType::intType()    },
        {"Score_String",       ValueType::decimalType()},
    });

    nts["GO_ID"] = createNodeType("GO_ID", {
        {"GO_ID",       ValueType::stringType()},
        {"Description", ValueType::stringType()},
        {"Category",    ValueType::stringType()},
    });

    nts["Homology"] = createNodeType("Homology", {
        {"Bitscore",  ValueType::decimalType()},
        {"End_1",     ValueType::intType()    },
        {"End_2",     ValueType::intType()    },
        {"Protein_1", ValueType::stringType() },
        {"Protein_2", ValueType::stringType() },
        {"Size",      ValueType::intType()    },
        {"Start_1",   ValueType::intType()    },
        {"Start_2",   ValueType::intType()    },
    });

    nts["Interaction"] = createNodeType("Interaction", {
        //{"Annotation",                 ValueType::listType()   },
        //{"CheckSum",                   ValueType::listType()   },
        //{"Created_At",                 ValueType::dateType()   },
        {"Coexpression_Score",           ValueType::intType()    },
        {"Coocurrence_Score",            ValueType::intType()    },
        {"Database_Score",               ValueType::intType()    },
        {"Experimental_Score",           ValueType::intType()    },
        //{"Expansion_Method",           ValueType::listType()   },
        {"Fusion_Score",                 ValueType::intType()    },
        {"Homology_Score",               ValueType::intType()    },
        {"Interaction_Detection_Method", ValueType::stringType() },
        {"Negative",                     ValueType::boolType()   },
        {"Neighborhood_Score",           ValueType::intType()    },
        //{"Parameters",                 ValueType::listType()   },
        {"Physical",                     ValueType::boolType()   },
        {"Protein_1",                    ValueType::stringType() },
        {"Protein_2",                    ValueType::stringType() },
        {"Score_Authors",                ValueType::decimalType()},
        {"Score_IntAct",                 ValueType::decimalType()},
        {"Score_String",                 ValueType::decimalType()},
        {"Score_Mentha",                 ValueType::decimalType()},
        {"Source",                       ValueType::stringType() },
        {"Stoichiometry_1",              ValueType::intType()    },
        {"Stoichiometry_2",              ValueType::intType()    },
        {"Type",                         ValueType::stringType() },
        //{"Updated_At",                 ValueType::dateType()   },
        //{"xRef",                       ValueType::listType()   },
    });

    nts["Metabolite"] = createNodeType("Metabolite", {
        {"Acceptor_Count", ValueType::stringType()      },
        //{"Alias",              ValueType::listType()  },
        //{"Alt_IDs",            ValueType::listType()  },
        {"AVG_Weight",           ValueType::intType()   },
        {"BiGG_ID",              ValueType::stringType()},
        {"BioCyc_ID",            ValueType::stringType()},
        {"Bioavailability",      ValueType::stringType()},
        {"Boiling_Point",        ValueType::stringType()},
        {"CAS_Number",           ValueType::stringType()},
        {"ChEBI_ID",             ValueType::stringType()},
        {"ChemSpider_ID",        ValueType::stringType()},
        //{"Created_At",         ValueType::dateType()  },
        {"Description",          ValueType::stringType()},
        {"DrugBank_ID",          ValueType::stringType()},
        {"Donor_Count",          ValueType::stringType()},
        {"FBOnto_ID",            ValueType::intType()   },
        //{"FooDB_ID",           ValueType::listType()  },
        {"Formal_Charge",        ValueType::stringType()},
        //{"Formula",            ValueType::listType()  },
        {"GeneBank_ID",          ValueType::stringType()},
        {"Ghose_Filter",         ValueType::boolType()  },
        {"HMDB_ID",              ValueType::stringType()},
        {"HMDB_Status",          ValueType::stringType()},
        {"InChI",                ValueType::stringType()},
        {"InChIKey",             ValueType::stringType()},
        {"IUPAC_Name",           ValueType::stringType()},
        {"KEGG_ID",              ValueType::stringType()},
        {"KNApSAcK_ID",          ValueType::stringType()},
        {"logP",                 ValueType::intType()   },
        {"logS",                 ValueType::intType()   },
        {"MDDR_Like_Rule",       ValueType::boolType()  },
        {"Melting_Point",        ValueType::intType()   },
        {"METLIN_ID",            ValueType::intType()   },
        {"Monoisotropic_Weight", ValueType::intType()   },
        {"Number_of_Rings",      ValueType::intType()   },
        {"Phenol_Explorer_ID",   ValueType::stringType()},
        {"Physiological_Charge", ValueType::stringType()},
        {"Polar_Surface_Area",   ValueType::stringType()},
        {"PDB_ID",               ValueType::stringType()},
        {"pKa_Strongest_Basic",  ValueType::intType()   },
        {"pKa_Strongest_Acidic", ValueType::intType()   },
        {"Polarizability",       ValueType::intType()   },
        {"PubChem_ID",           ValueType::stringType()},
        {"Refractivity",         ValueType::intType()   },
        {"Rotatable_Bond_Count", ValueType::intType()   },
        {"Rule_of_Five",         ValueType::boolType()  },
        {"SMILES",               ValueType::intType()   },
        {"Solubility",           ValueType::intType()   },
        //{"Updated_At",         ValueType::dateType()  },
        {"Veber_Rule",           ValueType::boolType()  },
        {"VMH_ID",               ValueType::intType()   },
        {"Water_Solubility",     ValueType::stringType()},
        {"WikiPedia_ID",         ValueType::stringType()},
        //{"xRef",               ValueType::listType()  },
    });

    nts["Mutation"] = createNodeType("Mutation", {
        {"beta_value",         ValueType::decimalType()},
        {"Chromosome",         ValueType::stringType() },
        {"Mutated_Sequence",   ValueType::stringType() },
        {"Mutation_ID",        ValueType::stringType() },
        {"p_value",            ValueType::decimalType()},
        {"Position",           ValueType::intType()    },
        {"Reference_Sequence", ValueType::stringType() },
    });

    nts["Measurement"] = createNodeType("Measurement", {
        {"Age",                 ValueType::stringType() },
        {"Concentration_Value", ValueType::decimalType()},
        {"Concentration_Units", ValueType::stringType() },
        {"Condition",           ValueType::decimalType()},
        {"Normal",              ValueType::intType()    },
        {"Sex",                 ValueType::stringType() },
    });

    nts["Organ"] = createNodeType("Organ", {
        {"Name", ValueType::stringType()},
    });

    nts["Orthology"] = createNodeType("Orthology", {
        //{"Annotation", ValueType::listType()  },
        {"End",          ValueType::intType()   },
        {"Protein_1",    ValueType::stringType()},
        {"Start",        ValueType::intType()   },
    });

    nts["Pathology"] = createNodeType("Pathology", {
        {"Name",    ValueType::stringType()},
        {"OMIM_ID", ValueType::stringType()},
    });

    nts["Pathway"] = createNodeType("Pathway", {
        {"Kegg_Map_ID", ValueType::stringType()},
        {"Name",        ValueType::stringType()},
        {"SMPDB_ID",    ValueType::stringType()},
    });

    nts["PDB_ID"] = createNodeType("PDB_ID", {
        {"PDB_ID", ValueType::stringType()},
    });

    nts["PFam"] = createNodeType("PFam", {
        {"Description", ValueType::stringType()},
        {"PFAM_ID",     ValueType::stringType()},
    });

    nts["Protein"] = createNodeType("Protein", {
        //{"Alias",                           ValueType::listType()  },
        //{"Alt_IDs",                         ValueType::listType()  },
        //{"Annotation",                      ValueType::listType()  },
        {"Biological_Role",                   ValueType::stringType()},
        {"CheckSum",                          ValueType::stringType()},
        //{"Created_At",                      ValueType::dateType()  },
        {"Description",                       ValueType::stringType()},
        {"Ensembl_ID",                        ValueType::stringType()},
        {"Experimental_Role",                 ValueType::stringType()},
        //{"Features",                        ValueType::listType()  },
        {"GeneBank_ID",                       ValueType::stringType()},
        {"General_Function",                  ValueType::stringType()},
        {"HMDB_ID",                           ValueType::stringType()},
        {"Identification_Method_Participant", ValueType::stringType()},
        {"Length",                            ValueType::intType()   },
        {"Molecular_Weight",                  ValueType::intType()   },
        {"Protein_ID",                        ValueType::stringType()},
        //{"Signalling_Regions",              ValueType::listType()  },
        {"Size",                              ValueType::intType()   },
        {"Specific_Function",                 ValueType::stringType()},
        {"Theoretical_PI",                    ValueType::stringType()},
        //{"Transmembrane_Regions",           ValueType::listType()  },
        {"Type",                              ValueType::stringType()},
        {"UniProt_ID",                        ValueType::stringType()},
        {"UniProt_Name",                      ValueType::stringType()},
        //{"Updated_At",                      ValueType::dateType()  },
        //{"xRef",                            ValueType::listType()  },
    });

    nts["Protein_Cluster"] = createNodeType("Protein_Cluster", {
        {"Cluster_ID",  ValueType::stringType()},
        {"Description", ValueType::stringType()},
        {"Size",        ValueType::intType()   },
    });

    nts["Publication"] = createNodeType("Publication", {
        //{"Authors",       ValueType::listType()  },
        //{"Created_At",    ValueType::dateType()  },
        //{"Date",          ValueType::dateType()  },
        {"doi",             ValueType::stringType()},
        {"IMEx_ID",         ValueType::stringType()},
        {"Journal",         ValueType::stringType()},
        {"MINT_ID",         ValueType::stringType()},
        {"Number",          ValueType::stringType()},
        {"Title",           ValueType::stringType()},
        {"PubMed_ID",       ValueType::stringType()},
        //{"Updated_At",    ValueType::dateType()  },
        {"Volume",          ValueType::intType()   },
        {"Year",            ValueType::intType()   },
    });

    nts["Sequence"] = createNodeType("Sequence", {
        {"Type",         ValueType::stringType()},
        {"Format",       ValueType::stringType()},
        {"Raw_Sequence", ValueType::stringType()},
    });

    nts["Taxonomy"] = createNodeType("Taxonomy", {
        {"NCBI_Tax_ID", ValueType::stringType()},
        {"Name",        ValueType::stringType()},
        {"String_Type", ValueType::stringType()},
        {"Type",        ValueType::stringType()},
    });

    nts["Tissue"] = createNodeType("Tissue", {
        {"Name", ValueType::stringType()},
    });

    nts["Transcript"] = createNodeType("Transcript", {
        {"CAGE_TPM",                        ValueType::decimalType()},
        {"Cell_Cycle_Dependency",           ValueType::boolType()   },
        {"Certainty",                       ValueType::stringType() },
        {"Count_TPM",                       ValueType::intType()    },
        {"Length",                          ValueType::intType()    },
        {"Level",                           ValueType::stringType() },
        {"Max_TPM",                         ValueType::decimalType()},
        {"Mean_TPM",                        ValueType::decimalType()},
        {"Min_TPM",                         ValueType::decimalType()},
        {"Normalized_CAGE_TPM",             ValueType::decimalType()},
        {"Normalized_TPM",                  ValueType::decimalType()},
        {"Protein_TPM",                     ValueType::decimalType()},
        {"Q1_TPM",                          ValueType::decimalType()},
        {"Q2_TPM",                          ValueType::decimalType()},
        {"Q3_TPM",                          ValueType::decimalType()},
        {"Read_Count",                      ValueType::decimalType()},
        {"Reliability",                     ValueType::stringType() },
        {"Scaled_CAGE_TPM",                 ValueType::decimalType()},
        {"Single_Cell_Variation_Intensity", ValueType::boolType()   },
        {"Single_Cell_Variation_Spatial",   ValueType::boolType()   },
        {"STD_TPM",                         ValueType::decimalType()},
    });

    createEdgeType(nts,
                   "Aliased_As",
                  {"Protein", "Taxonomy", "Metabolite"},
                  {"Alias"});
    createEdgeType(nts,
                   "Associated_with_Pathology",
                  {"Gene"},
                  {"Pathology"});
    createEdgeType(nts,
                   "Cited_In",
                  {"Interaction"},
                  {"Publication"});
    createEdgeType(nts,
                   "Encoded_By",
                  {"Protein", "Metabolite"},
                  {"Gene"});
    createEdgeType(nts,
                   "Expressed_In_Cell",
                  {"Gene", "Transcript", "Mutation"},
                  {"Cell"});
    createEdgeType(nts,
                   "Expressed_In_Tissue",
                  {"Gene", "Transcript", "Gene_Cluster"},
                  {"Tissue"});
    createEdgeType(nts,
                   "GC_Interacts_with_GC",
                  {"Gene_Cluster"},
                  {"GC_Interaction"});
    createEdgeType(nts,
                   "Located_Inside_Cell",
                  {"Protein", "Metabolite"},
                  {"Cellular_Location"});
    createEdgeType(nts,
                   "Measured_As",
                  {"Protein", "Metabolite"},
                  {"Measurement"});
    createEdgeType(nts,
                   "Measured_Inside",
                  {"Measurement"},
                  {"Tissue"});
    createEdgeType(nts,
                   "Member_of_Interaction",
                  {"Protein", "Metabolite"},
                  {"Interaction"});
    createEdgeType(nts,
                   "Part_of_Cluster",
                  {"Protein", "Protein_Cluster"},
                  {"Protein_Cluster"});
    createEdgeType(nts,
                   "Part_of_Homology",
                  {"Protein"},
                  {"Homology"});
    createEdgeType(nts,
                   "Part_of_Organ",
                  {"Tissue"},
                  {"Organ"});
    createEdgeType(nts,
                   "Part_of_Orthology",
                  {"Protein", "Orthology"},
                  {"Orthology", "Gene_Cluster"});
    createEdgeType(nts,
                   "Part_of_Pathway",
                  {"Protein", "Metabolite"},
                  {"Pathway"});
    createEdgeType(nts,
                   "Part_of_Pfam",
                  {"Protein"},
                  {"PFam"});
    createEdgeType(nts,
                   "Part_of_Taxonomy",
                  {"Protein", "Protein_Cluster", "Gene_Cluster",
                   "Interaction", "Transcript", "Homology", "Metabolite"},
                  {"Taxonomy"});
    createEdgeType(nts,
                   "Part_of_Tissue",
                  {"Tissue"},
                  {"Tissue"});
    createEdgeType(nts,
                   "Related_Clinical_Info",
                  {"Gene", "Pathology"},
                  {"Clinical_Info"});
    createEdgeType(nts,
                   "Related_with_GO_ID",
                  {"Protein"},
                  {"GO_ID"});
    createEdgeType(nts,
                   "Related_with_Term",
                  {"Protein", "Metabolite"},
                  {"Enrichment_Term"});
    createEdgeType(nts,
                   "Sequenced_As",
                  {"Protein", "Gene"},
                  {"Sequence"});
    createEdgeType(nts,
                   "Transcribed_As",
                  {"Gene"},
                  {"Transcript"});

    return true;
}

}
