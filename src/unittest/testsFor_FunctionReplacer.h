#ifndef TESTSFOR_FUNCTIONREPLACER_H
#define TESTSFOR_FUNCTIONREPLACER_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <assert.h>

/* Used for doReplace testing */
struct TestDefinition {
  FunctionReplacerSetup functionReplacerSetup;
  std::string originalCode;
  std::string expectedResultCode;
};

// FunctionReplacerSetups used during doReplaceTesting 
FunctionReplacerSetup funReplacerSetup_1 = {
std::string("original"),
0,
std::string("replacement()")
};

FunctionReplacerSetup funReplacerSetup_2 = {
std::string("original"),
1,
std::string("replacement(@1)")
};


FunctionReplacerSetup funReplacerSetup_3 = {
  std::string("original"),
  1,
  std::string("replacement()")
};

FunctionReplacerSetup funReplacerSetup_4 = {
  std::string("original"),
  1,
  std::string("NS::Replacement(@1)->methodCall()")
};

FunctionReplacerSetup funReplacerSetup_5 = {
  std::string("original"),
  3,
  std::string("replacement(@1, @2, @3)")
};

FunctionReplacerSetup funReplacerSetup_6 = {
  std::string("original"),
  3,
  std::string("NS::Replacement(@3)->methodCall(@1,@2)")
};

FunctionReplacerSetup funReplacerSetup_7 = {
  std::string("original"),
  3,
  std::string("NS::Replacement(@1)->methodCal(@2)")
};


static std::string unitTestDir("/cygdrive/c/KS/mw/EDG_Projects/CGIR_643/functionReplacer/src/unittest/");


template <typename T>
static std::string catStrNum(const std::string &prefix, T num)
{
  std::stringstream nameBuilder;
  nameBuilder << prefix << num;
  return nameBuilder.str();
}

static std::string getSrcDir(unsigned int srcNum)
{
  std::string srcDir = 
    catStrNum<unsigned int>(std::string("src_"), srcNum);
  srcDir.append("/");
  return srcDir;
}  

static std::string getSetupFileName(unsigned int setupNum)
{
  std::string setupFileName = 
    catStrNum<unsigned int>(std::string("replace_"), setupNum);
  setupFileName.append(".cpp");

  return setupFileName;
}

static FunctionReplacerSetup getFunReplacerSetupFromNum(unsigned int setupNum)
{

  switch(setupNum)
    {
    case 1:
      return funReplacerSetup_1;
      break;
    case 2:
      return funReplacerSetup_2;
      break;
    case 3:
      return funReplacerSetup_3;
      break;
    case 4:
      return funReplacerSetup_4;
      break;
    case 5:
      return funReplacerSetup_5;
      break;
    case 6:
      return funReplacerSetup_6;
      break;
    case 7:
      return funReplacerSetup_7;
      break;
    }

  std::cout << "Error : Invalid setupNum" << std::endl;
  assert(0);
}

static std::string getFileTextContents(const std::string &fileName)
{
  std::stringstream fileDump;

  std::ifstream file(fileName.c_str());
  
  if(!file.is_open()) {
    std::cout << "Error: Cannot open " << fileName << std::endl;
    assert(0);
  }

  file >> fileDump.rdbuf();
  return fileDump.str();

}

static TestDefinition buildTestDefinition(unsigned int srcNum, unsigned int setupNum)
{

  TestDefinition testDef;

  std::string srcDir = getSrcDir(srcNum);
  std::string setupFileName = getSetupFileName(setupNum);
  testDef.functionReplacerSetup = 
    getFunReplacerSetupFromNum(setupNum);

  /* Build Filenames */
  std::string filesRoot(unitTestDir);
  filesRoot.append(srcDir);
  
  std::string origSourceCodeFileName(filesRoot);
  origSourceCodeFileName.append("testSrc.cpp");

  std::string expectedResultFileName(filesRoot);
  expectedResultFileName.append(setupFileName);

  testDef.originalCode = getFileTextContents(origSourceCodeFileName);
  testDef.expectedResultCode = getFileTextContents(expectedResultFileName);

  return testDef;
}

#endif
