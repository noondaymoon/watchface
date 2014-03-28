//０:ヘッダ読み込み
#include "pebble.h"

//１:表示内容の定義

Window *window; //メインウインドウ
TextLayer *time_layer; //テキストレイヤ（時間:time_layer）
TextLayer *date_layer; //テキストレイヤ（日付:date_layer）
TextLayer *wday_layer;//テキストレイヤ（曜日:wday_layer）
Layer *line_layer; //ラインレイヤ（line_layer）

BitmapLayer *batt_layer; //画像レイヤ（バッテリー:batt_layer）
BitmapLayer *bt_layer; //画像レイヤ（bluetooth:bt_layer）

//２:アプリ設定
//ラインの表示（流用。ちゃんとわかってない）
void draw_line(Layer *layer, GContext* ctx) {
	graphics_context_set_fill_color(ctx, GColorWhite);
	graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);}

//時間等を呼び出してテキストレイヤへ代入する
static void handle_time_tick(struct tm* tick_time, TimeUnits units_changed) {
	
	//使う変数の定義
	static char time_text[] = "00:00"; //time_textには全部で5つの連続するアドレスを割り当てる
	static char date_text[] = "00/00/00"; //同様にdate_textには8つの連続するアドレスを割り当てる
	
	//時間
	char *time_format;
	//設定→時間表示で24/12時間表示を切り替えた場合に対応する
		if		 (clock_is_24h_style()) {time_format = "%R";} //24時間表示での時間を"00:00"の書式で返す
		else								 	  {time_format = "%I:%M";} //12時間表示での時間を"00:00"の書式で返す

	strftime(time_text, sizeof(time_text), time_format, tick_time);
	//時間を取得して代入する（ターゲット, アドレスのサイズ, 書式, 代入するべき取得したデータ）
	
	//12時間表示の場合、時間を格納するメモリのアドレスの1桁目を適切に処理する
	if  (!clock_is_24h_style() && (time_text[0] =='0'))
		//24時間表示ではない、かつtime_textの最初のアドレスが0の場合
		{memmove(time_text, &time_text[1], sizeof(time_text) - 1);}
		//time_textの2桁目の数字を1桁目に移動し、全体のアドレス数を1つ減らす
		//（これによって指定なしのtextlayerだと左に寄せられてしまうので右寄せが必要になる）
	
	text_layer_set_text(time_layer, time_text);
	//上記の展開により代入された時刻をtime_layerにテキストとして表示する
	
	//日付
	strftime(date_text, sizeof(date_text), "%d/%m/%y", tick_time); 
	//日付を取得して代入する（ターゲット, アドレスのサイズ, 書式, 代入するべき取得したデータ）
	text_layer_set_text(date_layer, date_text);
	//上記の展開により代入された日付をdate_layerにテキストとして表示する
	
	//曜日
	
	int wday_flag = tick_time -> tm_wday;
	//取得した時刻データから曜日を整数で取り出す（日曜日を0として6まで）
	if      	(wday_flag == 0) {text_layer_set_text(wday_layer, " sun");}
	else if  (wday_flag == 1) {text_layer_set_text(wday_layer, " mon");}
	else if  (wday_flag == 2) {text_layer_set_text(wday_layer, " tue");}
	else if  (wday_flag == 3) {text_layer_set_text(wday_layer, " wed");}
	else if  (wday_flag == 4) {text_layer_set_text(wday_layer, " thu");}
	else if  (wday_flag == 5) {text_layer_set_text(wday_layer, " fri");}
	else if  (wday_flag == 6) {text_layer_set_text(wday_layer, " sat");}
	//それぞれに対応する曜日を任意の文字列で定義してwday_layerにテキストとして表示する
	//（一文字目から打ち込んだらwedのwが飛んで表示されたので半角スペースで回避している）
	
}
//バッテリーの参照と画像の引当
//テスト
void handle_battery(BatteryChargeState charge) { //バッテリ情報の構造体のうち、充電状況（charge）を参照する
	if (charge.is_charging){ //（充電中の場合 - 充電中のアイコンを表示）
			if (charge.charge_percent > 88){ //（以下、充電していない場合 - 残量に適合するアイコンを表示）
		        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_32));}
			else if (charge.charge_percent > 81){
		        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_31));}
			else if (charge.charge_percent > 74){
		        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_30));}
			else if (charge.charge_percent > 67){
		        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_29));}
			else if (charge.charge_percent > 60){
		        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_28));}
			else if (charge.charge_percent > 53){
		        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_27));}
			else if (charge.charge_percent > 46){
		        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_26));}
			else if (charge.charge_percent > 39){
		        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_25));}
			else if (charge.charge_percent > 32){
		        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_24));}
			else if (charge.charge_percent > 25){
		        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_23));}
			else if (charge.charge_percent > 18){
		        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_22));}
			else if (charge.charge_percent > 11){
		        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_21));}
			else {
	        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_21));}
        }
	else if (charge.is_plugged){ //（満充電の場合 - 電池最大アイコンを表示）
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_12));}
	else if (charge.charge_percent > 88){ //（以下、充電していない場合 - 残量に適合するアイコンを表示）
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_12));}
	else if (charge.charge_percent > 81){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_11));}
	else if (charge.charge_percent > 74){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_10));}
	else if (charge.charge_percent > 67){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_9));}
	else if (charge.charge_percent > 60){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_8));}
	else if (charge.charge_percent > 53){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_7));}
	else if (charge.charge_percent > 46){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_6));}
	else if (charge.charge_percent > 39){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_5));}
	else if (charge.charge_percent > 32){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_4));}
	else if (charge.charge_percent > 25){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_3));}
	else if (charge.charge_percent > 18){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_2));}
	else if (charge.charge_percent > 11){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_1));}
	else if (charge.charge_percent > 0){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_0));}
    else{
	        // 無いとは思うけどコレ以外の状況が発生した場合に対応するため以下を記載（電池アイコンの表示なし）
		layer_set_hidden(bitmap_layer_get_layer(batt_layer), true);}
	//クラッシュする場合あった場合、各分岐にlayer_set_hidden(bitmap_layer_get_layer(batt_layer), false);を加える

}

/*
void handle_battery(BatteryChargeState charge) { //バッテリ情報の構造体のうち、充電状況（charge）を参照する
	if (charge.is_charging){ //（充電中の場合 - 充電中のアイコンを表示）
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_13));}
	else if (charge.is_plugged){ //（満充電の場合 - 電池最大アイコンを表示）
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_12));}
	else if (charge.charge_percent > 88){ //（以下、充電していない場合 - 残量に適合するアイコンを表示）
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_12));}
	else if (charge.charge_percent > 81){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_11));}
	else if (charge.charge_percent > 74){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_10));}
	else if (charge.charge_percent > 67){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_9));}
	else if (charge.charge_percent > 60){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_8));}
	else if (charge.charge_percent > 53){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_7));}
	else if (charge.charge_percent > 46){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_6));}
	else if (charge.charge_percent > 39){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_5));}
	else if (charge.charge_percent > 32){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_4));}
	else if (charge.charge_percent > 25){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_3));}
	else if (charge.charge_percent > 18){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_2));}
	else if (charge.charge_percent > 11){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_1));}
	else if (charge.charge_percent > 0){
        bitmap_layer_set_bitmap(batt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_0));}
    else{
	        // 無いとは思うけどコレ以外の状況が発生した場合に対応するため以下を記載（電池アイコンの表示なし）
		layer_set_hidden(bitmap_layer_get_layer(batt_layer), true);}
	//クラッシュする場合あった場合、各分岐にlayer_set_hidden(bitmap_layer_get_layer(batt_layer), false);を加える
}
*/



//bluetoothの接続状況参照と画像の引当

static void handle_bluetooth(bool connected){//bool（２択）。ここではconnected でtrueとしている？
	if		  (connected) {bitmap_layer_set_bitmap(bt_layer, gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_1));}
			  //画像レイヤに条件に対応する画像を配置（ターゲット, アップロード済みのファイルを画像に代入（該当ID））
	else						{bitmap_layer_set_bitmap(bt_layer,gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_0));
									vibes_long_pulse();
								}
								   //接続が失われた時点でバイブを動作させる一文を追加
}

//３:確保したメモリにを参照して表示する（ハンドラを借りてくる？）
static void do_init(void) {
	//基本ウインドウ
	window = window_create(); //基本ウインドウの配置
	window_stack_push(window, true); //通知刺し込み設定（通知を表示する（ターゲット, アニメーションするか））
	window_set_background_color(window, GColorBlack); //基本ウインドウの背景色設定（ターゲット, 背景色）
	
	Layer  *window_layer = window_get_root_layer(window); //宣言したwindowを親レイヤに定義する
	
	//ライン（line_layer）
	
	GRect line_frame = GRect(8, 120, 139, 2);
	line_layer = layer_create(line_frame); //宣言したレイヤをただのレイヤ（？）として詳細を設定する
	layer_set_update_proc(line_layer, draw_line); //自動的にレイヤを再描画する（ターゲット, 機能）
	layer_add_child(window_layer, line_layer); //親レイヤに子レイヤとして載せる
	
	//時間（ime_layer）
	
	time_layer = text_layer_create(GRect(8, 66, 110, 70)); //宣言したレイヤをテキストレイヤとして詳細を設定する
	text_layer_set_text_color(time_layer, GColorWhite); //テキストレイヤのテキスト色設定（ターゲット, 背景色）
	text_layer_set_background_color(time_layer, GColorClear); //背景色設定
	text_layer_set_font(time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_font_droid_44)));
	//テキストレイヤに表示するテキストのフォントを設定する
	//（ターゲット, アップロード済みのフォントを使用するコマンド（呼び出しコマンド（ID））
	text_layer_set_text_alignment(time_layer,GTextAlignmentRight); //テキスト表示位置の設定（ターゲット, 位置）
	//時間表示は標準だと左詰めのため、一桁だと左にズレてしまうため右寄せしている
	layer_add_child(window_layer, text_layer_get_layer(time_layer));
	
	//日付（date_layer）
	
	date_layer = text_layer_create(GRect(65, 125, 90, 25));
	text_layer_set_text_color(date_layer, GColorWhite);
	text_layer_set_background_color(date_layer, GColorClear);
	text_layer_set_font(date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_font_droid_18)));
	layer_add_child(window_layer, text_layer_get_layer(date_layer));
	
	//曜日（wday_layer）
	
	wday_layer = text_layer_create(GRect(60, 143, 90, 25));
	text_layer_set_text_color(wday_layer, GColorWhite);
	text_layer_set_background_color(wday_layer, GColorClear);
	text_layer_set_font(wday_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_font_droid_18)));
	layer_add_child(window_layer, text_layer_get_layer(wday_layer));
	
		//+時間のNULL回避（わかってない→調べる
	
	time_t now = time(NULL);
	struct tm *current_time = localtime(&now);
	handle_time_tick(current_time, SECOND_UNIT); //秒単位で時間を更新し、指定した先に代入する（？
	
	tick_timer_service_subscribe(SECOND_UNIT, &handle_time_tick); //代入された時間の情報を監視する（？
	
	//バッテリー（batt_layer）

	batt_layer = bitmap_layer_create(GRect(122, 151, 16, 10)); //画像レイヤの詳細を設定する
	layer_add_child(window_layer, bitmap_layer_get_layer(batt_layer)); //親レイヤに載せる 
	
	handle_battery(battery_state_service_peek()); //最新のバッテリ情報を参照し、指定した先に代入する（？
	battery_state_service_subscribe(&handle_battery); //代入されたバッテリ情報を監視する（？
	
	//bluetooth（bt_layer）

	bt_layer = bitmap_layer_create(GRect(106, 151, 11, 10));
	layer_add_child(window_layer, bitmap_layer_get_layer(bt_layer));

	handle_bluetooth(bluetooth_connection_service_peek()); //最新のbluetoothの情報を参照し、指定した先に代入する（？
	bluetooth_connection_service_subscribe(&handle_bluetooth); //代入されたbluetoothの情報を監視する（？

}
//４:メモリの開放（ハンドラの返却？）
static void do_deinit(void) {

	//ライン描画に使ったメモリの開放
	layer_destroy(line_layer); // 画像レイヤの開放（line_layer）
	
	//bluetoothに関する表示に使ったメモリの開放
	bluetooth_connection_service_unsubscribe(); //bluetoothの情報の監視を終了
	bitmap_layer_destroy(bt_layer); //画像レイヤの開放（bt_layer）

	//バッテリに関する表示に使ったメモリの開放
	battery_state_service_unsubscribe(); //バッテリ情報の監視を終了
	bitmap_layer_destroy(batt_layer); //画像レイヤの開放（batt_layer）

	//時間・日付の表示に関するに使ったメモリの開放
	tick_timer_service_unsubscribe(); //時間の監視を終了
	text_layer_destroy(wday_layer);	//テキストレイヤの開放（wday_layer）
	text_layer_destroy(date_layer);	//テキストレイヤの開放（day_layer）
	text_layer_destroy(time_layer);	//テキストレイヤの開放（time_layer）

	//メインウインドウの開放（window）
	window_destroy(window);

}

//５:繰り返し指示
int main(void) {
	do_init(); //２を繰り返す
	app_event_loop(); //３を繰り返す
	do_deinit(); //４を繰り返す
}