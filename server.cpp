#include <QNetworkReply>
#include <QHeaderView>
#include "server.hpp"
#include "gamelist.hpp"
#include "creategame.hpp"

Server::Server(ASIP& session_,MainWindow& mainWindow_) :
  session(session_),
  mainWindow(mainWindow_),

  vBoxLayout(this),
  newGame(tr("&New game")),
  refresh(tr("&Refresh page"))
{
  session.setParent(this);

  const QString gameListCategoryTitles[]={tr("My games"),tr("Invited games"),tr("Open games"),tr("Live games"),tr("Recent games")};
  for (const auto gameListCategory:session.availableGameListCategories()) {
    using namespace std;
    gameLists.insert(make_pair(gameListCategory,make_unique<GameList>(this,gameListCategoryTitles[gameListCategory])));
  }
  connect(&newGame,&QPushButton::clicked,this,&Server::createGame);
  connect(&refresh,&QPushButton::clicked,this,&Server::refreshPage);
  hBoxLayout.addWidget(&newGame);
  hBoxLayout.addWidget(&refresh);
  vBoxLayout.addLayout(&hBoxLayout);
  connect(&session,&ASIP::sendGameList,this,[&](const ASIP::GameListCategory gameListCategory,const std::vector<ASIP::GameInfo>& games) {
    const auto gameList=gameLists.find(gameListCategory);
    if (gameList!=gameLists.end())
      gameList->second->receiveGames(games);
  });
  refreshPage();
  for (auto& gameList:gameLists)
    vBoxLayout.addLayout(gameList.second.get());
}

Server::~Server()
{
  QNetworkReply* const networkReply=session.logout(nullptr);
  if (networkReply!=nullptr) {
    connect(networkReply,&QNetworkReply::finished,[=]{
      networkReply->deleteLater();
    });
  }
}

void Server::refreshPage() const
{
  session.state();
}

void Server::createGame()
{
  const auto createGame=new CreateGame(session,*this);
  createGame->setAttribute(Qt::WA_DeleteOnClose);
  createGame->show();
}

void Server::resizeEvent(QResizeEvent*)
{
  for (const auto& gameList:gameLists) {
    gameList.second.get()->tableWidget.horizontalHeader()->setStretchLastSection(false);
    gameList.second.get()->tableWidget.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    gameList.second.get()->tableWidget.horizontalHeader()->setStretchLastSection(true);
    gameList.second.get()->tableWidget.horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
  }
}
