
#include "gtest/gtest.h"
#include "rtRoutingTree.h"

extern "C" rtBool rtRoutingTree_IsTopicMatch(char const* topic, char const* exp);
extern "C" int    rtRoutingTree_GetSize(rtRoutingTree* self);

static rtBool ContextIsEven(rtRoutingTreeEntry* e, void* user_data)
{
  (void) user_data;

  intptr_t p = (intptr_t) e->context;
  return (p % 2) == 0
    ? rtBool_TRUE
    : rtBool_FALSE;
}

static rtBool ContextIsOdd(rtRoutingTreeEntry* e, void* user_data)
{
  (void) user_data;

  intptr_t p = (intptr_t) e->context;
  return (p % 2) != 0
    ? rtBool_TRUE
    : rtBool_FALSE;
}

static rtError ForEachCallback(rtRoutingTreeEntry* e, void* user_data)
{
  (void) e;
  printf("here:%p\n", user_data);
  EXPECT_EQ(user_data, (void *) 0xdeadbeef);
  return RT_OK;
}


TEST(rtRoutingTree, Match1) {
  EXPECT_TRUE(rtRoutingTree_IsTopicMatch(
    "A",
    "A"));
}

TEST(rtRoutingTree, Match2) {
  EXPECT_TRUE(rtRoutingTree_IsTopicMatch(
    "A.B.C.D.E.F.G",
    "A.B.C.D.E.F.G"));
}

TEST(rtRoutingTree, Wildcard1) {
  EXPECT_TRUE(rtRoutingTree_IsTopicMatch(
    "A.B",
    "A.*"));
}

TEST(rtRoutingTree, Wildcard2) {
  EXPECT_TRUE(rtRoutingTree_IsTopicMatch(
    "A.B.C.D.E.F.G",
    "A.B.C.D.E.F.*"));
}

TEST(rtRoutingTree, Wildcard3) {
  EXPECT_TRUE(rtRoutingTree_IsTopicMatch(
    "A.B.C.D",
    "A.*.*.D"));
}

TEST(rtRoutingTree, Wildcard4) {
  EXPECT_TRUE(rtRoutingTree_IsTopicMatch(
    "A.B.C.D",
    "A.>"));
}

TEST(rtRoutingTree, Wildcard5) {
  EXPECT_TRUE(rtRoutingTree_IsTopicMatch(
    "A.B.C.D",
    "A.B.>"));
}

TEST(rtRoutingTree, Wildcard6) {
  EXPECT_TRUE(rtRoutingTree_IsTopicMatch(
    "A.B.C.D",
    "A.*.C.>"));
}

TEST(rtRoutingTree, Init) {
  rtRoutingTree* t = rtRoutingTree_New();
  rtRoutingTree_Delete(t);
}

TEST(rtRoutingTree, Size) {
  rtRoutingTree* t = rtRoutingTree_New();
  for (int i = 0; i < 100; ++i)
  {
    intptr_t p = i;
    rtRoutingTree_AddRoute(t, "A.B.C", (void *) p);
  }
  rtRoutingTree_Delete(t);
}

TEST(rtRoutingTree, RemoveIf) {
  rtRoutingTree* t = rtRoutingTree_New();
  for (int i = 0; i < 100; ++i)
  {
    intptr_t p = i;
    rtRoutingTree_AddRoute(t, "A.B.C", (void *) p);
  }
  EXPECT_EQ(rtRoutingTree_RemoveIf(t, ContextIsEven, NULL), RT_OK);
  EXPECT_EQ(rtRoutingTree_GetSize(t), 50);
  EXPECT_EQ(rtRoutingTree_RemoveIf(t, ContextIsOdd, NULL), RT_OK);
  EXPECT_EQ(rtRoutingTree_GetSize(t), 0);
  rtRoutingTree_Delete(t);
}


TEST(rtRoutingTree, ForEach) {
  rtRoutingTree* t = rtRoutingTree_New();
  for (int i = 0; i < 100; ++i)
  {
    intptr_t p = i;
    rtRoutingTree_AddRoute(t, "A.B.C", (void *) p);
  }

  EXPECT_EQ(rtRoutingTree_GetSize(t), 100);
  EXPECT_EQ(rtRoutingTree_ForEach(t, "A.B.C", ForEachCallback, (void *) 0xdeadbeef), RT_OK);
  rtRoutingTree_Delete(t);
}

TEST(rtRoutingTree, ForEachWithTopicCheck) {
  rtRoutingTree* t = rtRoutingTree_New();
  rtRoutingTree_AddRoute(t, "A.B.*.D", nullptr);
  rtRoutingTree_AddRoute(t, "A.B.C", nullptr);

  rtRoutingTree_ForEach(t, "A.B.C.D", [](rtRoutingTreeEntry* e, void* /*userp*/) -> rtError {
    EXPECT_STREQ(e->expression, "A.B.*.D");
    return RT_OK;
  }, nullptr);

  rtRoutingTree_ForEach(t, "A.B.C", [](rtRoutingTreeEntry* e, void* /*userp*/) -> rtError {
    EXPECT_STREQ(e->expression, "A.B.C");
    return RT_OK;
  }, nullptr);

  rtRoutingTree_Delete(t);
}
