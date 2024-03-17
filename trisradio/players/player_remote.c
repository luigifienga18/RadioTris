#include "player_remote.h"

#define REQ_2_SEND_ACTION 170
#define ACTION_RCVD 85
#define STATE_RCVD 243

static uint8_t DEFAULT_RFD_CHANNEL[] = {'s','l','v','v','3'};

void playerRemote_init(playerRemote_t *p, playerId id, RFDriver * rfp)
{
  p->player.id = id;
  p->rfp = rfp;
}

void playerRemote_deInit(playerRemote_t *p)
{
  (void)p;
}

void playerRemote_getAction(playerRemote_t *p, action_t *action)
{
  RFTxFrame txbuff;
  RFRxFrame rxbuff;
  rf_msg_t rcvmsg;

  for(unsigned char i=0; i<5; i++)
  {
    txbuff.tx_address[i] = DEFAULT_RFD_CHANNEL[i];
    rxbuff.rx_address[i] = DEFAULT_RFD_CHANNEL[i];
  }

  txbuff.tx_paylen = 1;
  txbuff.tx_payload[0] = REQ_2_SEND_ACTION;

  rxbuff.rx_paylen = sizeof(action_t);

  do
  {
    rcvmsg = rfTransmit(p->rfp, 1, &txbuff, TIME_MS2I(75));
  }
  while(rcvmsg != RF_OK);

  //do
  //{
    rcvmsg = rfReceive(p->rfp, 1, &rxbuff, TIME_INFINITE);
  //}
  //while(rcvmsg != RF_OK);

  action_t *a = (action_t *)rxbuff.rx_payload;

  action->player = a->player;
  action->typeOfAction = a->typeOfAction;
}

void playerRemote_sendGameState(playerRemote_t *p, gameStatus_t *game)
{
  RFTxFrame txbuff;
  rf_msg_t rcvmsg;

  for(unsigned char i=0; i<5; i++)
    txbuff.tx_address[i] = DEFAULT_RFD_CHANNEL[i];

  txbuff.tx_paylen = sizeof(gameStatus_t) <= RF_PAYLEN ? sizeof(gameStatus_t) : RF_PAYLEN;
  uint8_t *g = (uint8_t *)game;
  for(unsigned char i=0; i < txbuff.tx_paylen; i++)
    txbuff.tx_payload[i] = g[i];

  do
  {
    rcvmsg = rfTransmit(p->rfp, 1, &txbuff, TIME_MS2I(75));
  }
  while(rcvmsg != RF_OK);
}

// Slave functions

void playerRemote_waitForReq2SendAction(playerRemote_t *p)
{
  RFRxFrame rxbuff;
  //rf_msg_t rcvmsg;

  for(unsigned char i=0; i<5; i++)
    rxbuff.rx_address[i] = DEFAULT_RFD_CHANNEL[i];

  rxbuff.rx_paylen = 1;

  //do
  //{
  /*rcvmsg = */rfReceive(p->rfp, 1, &rxbuff, TIME_INFINITE);
  //}
  //while(rcvmsg != RF_OK || rxbuff.rx_payload[0] != REQ_2_SEND_ACTION);
}

void playerRemote_sendAction(playerRemote_t *p, action_t *action)
{
  RFTxFrame txbuff;
  rf_msg_t rcvmsg;

  for(unsigned char i=0; i<5; i++)
    txbuff.tx_address[i] = DEFAULT_RFD_CHANNEL[i];

  txbuff.tx_paylen = sizeof(action_t) <= RF_PAYLEN ? sizeof(action_t) : RF_PAYLEN;

  uint8_t *a = (uint8_t *)action;
  for(unsigned char i=0; i < txbuff.tx_paylen; i++)
    txbuff.tx_payload[i] = a[i];

  do
  {
    rcvmsg = rfTransmit(p->rfp, 1, &txbuff, TIME_MS2I(75));
  }
  while(rcvmsg != RF_OK);
}

void playerRemote_getGameState(playerRemote_t *p, gameStatus_t *game)
{
  RFRxFrame rxbuff;
  //rf_msg_t rcvmsg;

  for(unsigned char i=0; i<5; i++)
    rxbuff.rx_address[i] = DEFAULT_RFD_CHANNEL[i];

  rxbuff.rx_paylen = sizeof(gameStatus_t) <= RF_PAYLEN ? sizeof(gameStatus_t) : RF_PAYLEN;

  //do
  //{
    /*rcvmsg = */rfReceive(p->rfp, 1, &rxbuff, TIME_INFINITE);
  //}
  //while(rcvmsg != RF_OK);

  uint8_t *g = (uint8_t *)game;
  for(unsigned char i=0; i < rxbuff.rx_paylen; i++)
    g[i] = rxbuff.rx_payload[i];
}
