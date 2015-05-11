#include "realclients.h"
using namespace integration;

bool RealClientBD::CheckHosts(IP &){
    //вот сюда надо положить функцию клиента БД, возрващающую IP упавшего хоста, если кто-то упал
    return true;
}

void RealClientBD::SendConfig(){
    // отправить изменённый конфигурационный файл SendConfig()
}

void RealClientBD::UpdateConfig(){}

//---------------------------------------------------------------------------------------------------

bool RealClientDisp::CheckHosts(IP &){
    //вот сюда надо положить функцию клиента Диспетчера, возрващающую IP упавшего хоста, если кто-то упал
    return true;
}

void RealClientDisp::SendConfig(){
    // отправить изменённый конфигурационный файл SendConfig()
}

void RealClientDisp::UpdateConfig(){}

//---------------------------------------------------------------------------------------------------

bool RealClientRBT::CheckHosts(IP &){
    //вот сюда надо положить функцию клиента RBTree, возрващающую IP упавшего хоста, если кто-то упал
    return true;
}

void RealClientRBT::SendConfig(){
    // отправить изменённый конфигурационный файл SendConfig()
}

void RealClientRBT::UpdateConfig(){}

