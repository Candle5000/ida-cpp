#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

//アイテムデータの構造体
typedef struct {
	int id;
	int name_length;
	char name[256];
	int text_length;
	char text[256];
	int tag1;
	int tag2;
	int tag3;
} itemdata_t;

/*****
 * 文字列をファイルに書き込む
 */
void m_fout(char *str, char *outfile) {
	ofstream fout( outfile, ios::out | ios::app );
	fout << str << endl;
	fout.close();
}

/*****
 * アイテム1個分のデータを読み込む
 */
itemdata_t readItemData( ifstream *finp ) {
	itemdata_t item = {0, 0, "", 0, "", 0, 0, 0};
	memset( item.name, '\0', 256 );
	memset( item.text, '\0', 256 );
	
	finp->read( ( char * ) &item.id, 2 ); //ID読み込み
	if( item.id == 0 ) return item; //IDが存在しない場合終了
	finp->read( ( char * ) &item.name_length, 1 ); //名前の長さ読み込み
	finp->read( ( char * ) &item.name, item.name_length ); //名前読み込み
	finp->read( ( char * ) &item.text_length, 1 ); //テキストの長さ読み込み
	finp->read( ( char * ) &item.text, item.text_length ); //テキスト読み込み
	finp->read( ( char * ) &item.tag1, 1 ); //タグ1読み込み
	finp->read( ( char * ) &item.tag2, 4 ); //タグ2(2～5)読み込み
	finp->read( ( char * ) &item.tag3, 1 ); //タグ3(6)読み込み
	
	return item;
}

/*****
 * アイテム1個分のデータを出力する
 */
void writeData(itemdata_t item, char *outfile ) {
	ofstream fout( outfile, ios::out | ios::app );
	//  書き込み用ファイルを開く
	//  ios::out は書き込み専用  ios::app は追加出力
	if(item.id == 0) return;
	fout << "ID : " << item.id << endl;
	fout << item.name << endl;
	fout << item.text << endl;
	if( ( item.tag1 / 1 ) % 2 ) fout << "RARE ";
	if( ( item.tag1 / 2 ) % 2 ) fout << "NOTRADE ";
	if( ( item.tag1 / 4 ) % 2 ) fout << "売却不可 ";
	if( ( item.tag1 / 8 ) % 2 ) fout << "破棄不可 ";
	fout << endl;
	fout << "Tag3:" << item.tag2 ;
	fout << " Stack:" << item.tag3 << "\n" << endl;
	fout.close();
}

/*****
 * データベース登録用データを出力する
 */
void writePHPData(itemdata_t item, char *outfile ) {
	ofstream fout( outfile, ios::out | ios::app );
	//  書き込み用ファイルを開く
	//  ios::out は書き込み専用  ios::app は追加出力
	if(item.id == 0) return;
	fout << "ID" << item.id << endl;
	fout << item.name << endl;
	fout << item.text << "EOT" << endl;
	fout << item.tag1 << endl;
	fout << item.tag3 << endl;
	fout.close();
}

/*****
 * 全データ出力モード
 */
void readAllData() {
	char infile[] = "item.dat";
	char outfile[] = "item.txt";
	ifstream *fin;
	int num = 0;
	itemdata_t item = {0, 0, "", 0, "", 0, 0, 0};
	
	fin = new ifstream( infile, ios::in | ios::binary );
	//  読み込み用ファイルを開く
	//  ios::in は読み込み専用  ios::binary はバイナリ形式
	
	ofstream fout( outfile, ios::out | ios::trunc );
	//  書き込み用ファイルを開く
	//  ios::out は書き込み専用  ios::trunc は上書き
	
	if ( !*fin ){
		cout << "error : cannot open file (item.dat)" << endl;
		return;
	}
	if ( !fout ){
		cout << "error : cannot open file (item.txt)" << endl;
		return;
	}
	//  ファイルが開けなかったときの対策
	
	fin->read( ( char * ) &num, 2 );
	fout << "Registered Items : " << num << "\n" << endl;
	fout.close();
	//登録数のみ読み込み
	
	do {
		item = readItemData( fin );
		writeData( item, outfile );
	} while( item.id > 0 );
	
	fin->close();
	//ファイルを閉じる
}

/*****
 * 更新データ出力モード
 */
void checkUpdate() {
	char infileB[] = "./before/item.dat";
	char infileA[] = "./after/item.dat";
	char outfile[] = "update.txt";
	char bastr[] = "　↓\n";

	ifstream *finB, *finA;
	int num1 = 0;
	int num2 = 0;
	itemdata_t itemB = {0, 0, "", 0, "", 0, 0, 0};
	itemdata_t itemA = {0, 0, "", 0, "", 0, 0, 0};
	
	finB = new ifstream( infileB, ios::in | ios::binary );
	finA = new ifstream( infileA, ios::in | ios::binary );
	//  読み込み用ファイルを開く
	//  ios::in は読み込み専用  ios::binary はバイナリ形式
	
	ofstream fout( outfile, ios::out | ios::trunc );
	//  書き込み用ファイルを開く
	//  ios::out は書き込み専用  ios::trunc は上書き
	
	if ( !*finB ){
		cout << "error : cannot open file (before/item.dat)" << endl;
		return;
	}
	if ( !*finA ){
		cout << "error : cannot open file (after/item.dat)" << endl;
		return;
	}
	if ( !fout ){
		cout << "error : cannot open file (item.txt)" << endl;
		return;
	}
	//  ファイルが開けなかったときの対策
	
	finB->read( ( char * ) &num1, 2 );
	finA->read( ( char * ) &num2, 2 );
	if( num1 >= num2 ) {
		cout << "error : before/item.dat contains too many items" << endl;
		return;
	}
	fout << "Added Items : " << ( num2 - num1 ) << "\n" << endl;
	fout.close();
	//登録数のみ読み込み
	
	int count = 0;
	do {
		itemB = readItemData(finB);
		itemA = readItemData(finA);
		if ((itemB.id == itemA.id) && memcmp(&itemB, &itemA, sizeof(itemdata_t)) != 0) {
			writeData(itemB, outfile);
			m_fout(bastr, outfile);
			writeData(itemA, outfile);
			count++;
		}
		while (itemB.id != itemA.id) {
			writeData(itemA, outfile);
			itemA = readItemData(finA);
		}
	} while ((itemB.id > 0) && (itemA.id > 0));
	
	ofstream fout2(outfile, ios::out | ios::app);
	fout2 << "Modified Items : " << count << "\n" << endl;
	fout2.close();
	
	finB->close();
	finA->close();
	//ファイルを閉じる
}

/*****
 * データベース登録用出力モード
 */
void phpData() {
	char infile[] = "item.dat";
	char outfile[] = "database.txt";
	ifstream *fin;
	int num = 0;
	itemdata_t item = {0, 0, "", 0, "", 0, 0, 0};
	
	fin = new ifstream( infile, ios::in | ios::binary );
	//  読み込み用ファイルを開く
	//  ios::in は読み込み専用  ios::binary はバイナリ形式
	
	ofstream fout( outfile, ios::out | ios::trunc );
	//  書き込み用ファイルを開く
	//  ios::out は書き込み専用  ios::trunc は上書き
	
	if ( !*fin ){
		cout << "error : cannot open file (item.dat)" << endl;
		return;
	}
	if ( !fout ){
		cout << "error : cannot open file (database.txt)" << endl;
		return;
	}
	//  ファイルが開けなかったときの対策
	
	fin->read( ( char * ) &num, 2 );
	fout << "ITEM" << num << endl;
	fout.close();
	//登録数のみ読み込み
	
	do {
		item = readItemData( fin );
		writePHPData( item, outfile );
	} while( item.id > 0 );
	
	fin->close();
	//ファイルを閉じる
}

/*****
 * メイン関数
 */
int main( int argc, char *argv[] ) {
	if( argc > 2 ) {
		cout << "error : undefined key" << endl;
	} else if( ( argc == 2 ) && ( strlen( argv[1] ) > 1 ) ) {
		cout << "error : undefined key" << endl;
	} else if( argc == 2 ) {
		switch( argv[1][0] ) {
			case 'r':
				readAllData();
				break;
			case 's':
				checkUpdate();
				break;
			case 'p':
				phpData();
				break;
			default:
				cout << "error : undefined key" << endl;
				return 1;
				break;
		}
	} else {
		readAllData();
	}
	
	return 0;
}
