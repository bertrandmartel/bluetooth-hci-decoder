/**
 * The MIT License (MIT)
 * <p/>
 * Copyright (c) 2015 Bertrand Martel
 * <p/>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * <p/>
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * <p/>
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
package fr.bmartel.android.hcidecoder;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 * Test activity for using hcidecoder library from Android
 *
 * @author Bertrand Martel
 */
public class MainActivity extends Activity {

    private String TAG = MainActivity.this.getClass().getName();

    private final static String HCI_FILE_PATH = "/sdcard/btsnoop_hci.log";

    public native String decodeHci(String snoopFile);

    static {
        System.loadLibrary("hciwrapper");
    }

    public void moveRawFile(int fileId, String fileName) {
        try {
            InputStream in = getApplicationContext().getResources().openRawResource(fileId);
            FileOutputStream out = new FileOutputStream(fileName);
            byte[] buff = new byte[1024];
            int read = 0;

            try {
                while ((read = in.read(buff)) > 0) {
                    out.write(buff, 0, read);
                }
            } finally {
                in.close();
                out.close();
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        moveRawFile(R.raw.btsnoophci, HCI_FILE_PATH);

        Button button = (Button) findViewById(R.id.hci_decode_btn);

        final TextView text = (TextView) findViewById(R.id.hci_json_txt);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                final String response = decodeHci(HCI_FILE_PATH);

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        text.setText(response);
                    }
                });
            }
        });

        Button clear_button = (Button) findViewById(R.id.clear_btn);

        clear_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        text.setText("");
                    }
                });
            }
        });
    }
}
