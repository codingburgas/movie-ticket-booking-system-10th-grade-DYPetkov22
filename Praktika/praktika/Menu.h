#pragma once

#include "AdminManager.h"
#include "MovieManager.h"
#include "UserManager.h"

void adminFeatures(MovieManager& movieManager, bool isMainAdmin, AdminManager& adminManager);
void adminLoginFlow(AdminManager& adminManager, MovieManager& movieManager);
void userLoginFlow(UserManager& userManager, MovieManager& movieManager);
void guestMenu(MovieManager& movieManager);
void loginPage();
void runSystem();


