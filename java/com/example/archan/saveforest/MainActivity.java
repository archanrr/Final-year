package com.example.archan.saveforest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttMessageListener;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.io.UnsupportedEncodingException;
import java.lang.annotation.Annotation;

import static com.example.archan.saveforest.contraints.flame;
import static com.example.archan.saveforest.contraints.gas;
import static com.example.archan.saveforest.contraints.prob;
import static com.example.archan.saveforest.contraints.status;
import static com.example.archan.saveforest.contraints.temp;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final TextView tempText = (TextView) findViewById (R.id.temp);
        final TextView gasText = (TextView) findViewById (R.id.gas);
        final TextView flameText = (TextView) findViewById (R.id.flame);
        final TextView statusText = (TextView) findViewById (R.id.status);
        final TextView probText = (TextView) findViewById (R.id.prob);


        String clientId = MqttClient.generateClientId();
        contraints constants=new contraints();
        final MqttAndroidClient client = new MqttAndroidClient(getApplicationContext(), constants.MQTT_BROKER_URL,clientId);


            client.setCallback(new MqttCallbackExtended() {
                @Override
                public void connectComplete(boolean b, String s) {
                    Log.w("mqtt", s);
                }

                @Override
                public void connectionLost(Throwable throwable) {

                }

                @Override
                public void messageArrived(String topic, MqttMessage mqttMessage) throws Exception {
                   // Log.w("Mqtt", mqttMessage.toString());
                    //Toast.makeText(MainActivity.this,  topic+":"+mqttMessage.toString(), Toast.LENGTH_SHORT).show();
                    if (topic.equals("archan/temperature"))
                    {
                        //Toast.makeText(MainActivity.this,  topic+":"+mqttMessage.toString(), Toast.LENGTH_SHORT).show();

                        tempText.setText(mqttMessage.toString());
                    }
                    else if(topic.equals("archan/gas"))
                    {
                        //Toast.makeText(MainActivity.this,  topic+":"+mqttMessage.toString(), Toast.LENGTH_SHORT).show();

                        gasText.setText(mqttMessage.toString());
                    }
                    else if(topic.equalsIgnoreCase("archan/flame"))
                    {
                        //Toast.makeText(MainActivity.this,  topic+":"+mqttMessage.toString(), Toast.LENGTH_SHORT).show();

                        if(mqttMessage.toString().equalsIgnoreCase("1"))
                        flameText.setText("NO flame");

                        else
                        {
                            flameText.setText("flame");
                        }
                    }
                    else if(topic.equalsIgnoreCase("archan/status"))
                    {
                        statusText.setText(mqttMessage.toString());
                    }
                    else if(topic.equals("archan/prob"))
                    {
                        probText.setText(mqttMessage.toString()+"% chance");
                    }
                }

                @Override
                public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {

                }
            });

        try {
            client.connect().setActionCallback(new IMqttActionListener() {
                    @Override

                    public void onSuccess(IMqttToken asyncActionToken) {
                        // We are connected
                        Toast.makeText(MainActivity.this, "Connected to Mqtt Broker!!!!!!", Toast.LENGTH_SHORT).show();

                            SubscribeTopic(client, contraints.temp);
                            SubscribeTopic(client,flame);
                            SubscribeTopic(client,gas);
                            SubscribeTopic(client,status);
                            SubscribeTopic(client,prob);

                    }
                    @Override
                    public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                        // Something went wrong e.g. connection timeout or firewall problems
                        Toast.makeText(MainActivity.this, "Connection to Mqtt Broker Failed", Toast.LENGTH_SHORT).show();
                    }
                });
        } catch (MqttException e1) {
            e1.printStackTrace();
        }
    }
    public void SubscribeTopic(MqttAndroidClient client, final String  topic) {
        try {
            //Toast.makeText(this, "try to subscribe............", Toast.LENGTH_SHORT).show();
            client.subscribe(topic, 0, null, new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                   // Log.w("Mqtt", "Subscribed!");
                    Toast.makeText(MainActivity.this, "Subscribed!!!!!!", Toast.LENGTH_SHORT).show();
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    //Log.w("Mqtt", "Subscribed fail!");
                    Toast.makeText(MainActivity.this, "Subscribed fail!", Toast.LENGTH_SHORT).show();
                }

            });

        } catch (MqttException ex) {
            System.err.println("Exceptionst subscribing");
            ex.printStackTrace();
        }
    }

}
