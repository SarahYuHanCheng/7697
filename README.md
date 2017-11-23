11/16 Ｗ9
1.  期中考
	•	5 成績確認  分組確認
	•	5 觀察記錄分享 問卷分享
	•	5 過關心得
	•	5 積分戰Ｉ II 規則  相撲競賽
2.  TCP UDP
	▪	概念講解(TCP,UDP// AP, STA mode )
	▪	程式碼
	▪	實作練習
3. 系統使用
* 連線程式碼
* 系統操作

宣達:

下週事項宣達：
	⁃	約時間，一晚兩組
下週準備：
	1.	手機app


積分II
server :
�1. 廣播”start” 
�2. 可匯入助教設定的寶箱位置(含ID)
3. 當車子request時，回傳寶箱ID
4. 可判斷車子到達終點(成功開寶箱，可以的話介面上顯示每台車所花的秒數）
3. 最後一台車到達終點後，遊戲結束

以下補充說明：
	⁃	BSP就是將在CPU週邊相關的硬體,
寫driver(如:ethernet, flash file system,console port...)
給standard library或OS來控制(如:printf,socket,fopen...)

	•	FreeRTOS有時會被視為是一個『執行緒函式庫』而非『作業系統』
	•	http://www.FreeRTOS.org/Documentation
	•	task

每個task會使用RAM空間來記錄狀態，
Each task requires RAM that is used to hold the task state, and used by the task as its stack. If a task is created using xTaskCreate() then the required RAM is automatically allocated from the FreeRTOS heap. If a task is created using xTaskCreateStatic() then the RAM is provided by the application writer, so can be statically allocated at compile time.

	•	來談談OS吧，
	•	OS記憶體主要分為Global, stack和heap。global 存放在最高位址，包括全域變數，靜態變數。
	•	接著global下面是stack，function的address, return的value, local variable. 凡是在compile時期就可以預知從何時開始配置跟何時結束回收的資料，都會往stack存放。再來是heap，heap由malloc 或 new來配置。回收則是由programmer決定
	•	

stack overflow一般是因為過多的函式呼叫(例如：遞迴太深)、或區域變數使用太多，此時請試著將stack size調大一點，另外檢查看看函式的呼叫跟變數的使用量。反之，當發生heap overflow請檢查是否都有正確將heap space的資料回收，另外採行的動態配置是否合理


程式就會依照上次所執行的 s 字串繼續比對下去，等到 *last 被指向 NULL 的時候就不會在執行 strtok 了

“ \r 回車”（carriage return）和“ \n 換行”（line feed）
Unix系統裡，每行結尾只有“<換行>”，即“\n”；Windows系統裡面，每行結尾是“<換行><回車>”，即“\n\r”；Mac系統裡，每行結尾是“<回車>”。一個直接後果是，Unix/Mac系統下的文件在Windows裡打開的話，所有文字會變成一行；而Windows裡的文件在Unix/Mac下打開的話，在每行的結尾可能會多出一個^M符號。
