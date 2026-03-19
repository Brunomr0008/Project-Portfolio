package pt.meec.wcom.HomeMenu

import android.annotation.SuppressLint
import android.content.Intent
import android.os.Bundle
import android.text.InputType
import android.view.View
import android.widget.EditText
import android.widget.ImageButton
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import pt.meec.wcom.R
import pt.meec.wcom.MainMenu.Welcome

import com.google.firebase.Firebase
import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.auth.auth
import com.google.firebase.firestore.FirebaseFirestore
import com.google.firebase.firestore.firestore
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import pt.meec.wcom.Room.Room

class Login : AppCompatActivity() {
    // Variable to keep track of password visibility state
    private var isPasswordVisible = false

    // Declare variables as proprieties of a class
    private lateinit var editTextEmail: EditText
    private lateinit var editTextPassword: EditText
    private lateinit var togglePasswordButton: ImageButton

    // Initialize Firebase Auth and Firestore
    private lateinit var auth: FirebaseAuth // declare_auth
    private lateinit var db: FirebaseFirestore

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_login)

        auth = Firebase.auth  // Initialize Firebase Auth
        db = Firebase.firestore

        // -------------- Initialize the Variables -------------------
        // Use findViewById to connect the components to layout
        editTextEmail = findViewById(R.id.emailEditText)
        editTextPassword = findViewById(R.id.passwordEditText)
        togglePasswordButton = findViewById(R.id.togglePasswordButton)  // Toggle visibility password
    }

    // Toggles password visibility when the toggle button is clicked
    fun togglePasswordButtonLogin(view: View) {
        if (isPasswordVisible) {
            // Hide the password
            editTextPassword.inputType = InputType.TYPE_CLASS_TEXT or InputType.TYPE_TEXT_VARIATION_PASSWORD
            togglePasswordButton.setImageResource(R.drawable.visibility_off)
        } else {
            // Show the password
            editTextPassword.inputType = InputType.TYPE_CLASS_TEXT
            togglePasswordButton.setImageResource(R.drawable.visibility)
        }
        // Move the cursor to the end of the text
        editTextPassword.setSelection(editTextPassword.text.length)
        isPasswordVisible = !isPasswordVisible
    }

    fun loginOnClickBtn(view: View) {
        val email = editTextEmail.text.toString()
        val password = editTextPassword.text.toString()

        if (email.isNotEmpty() && password.isNotEmpty()) {
            signIn(email, password)
        } else if (email.isEmpty()) {
            editTextEmail.error = "Email required"
        } else if (password.isEmpty()) {
            editTextPassword.error = "Password required"
        }
    }

    // Triggered when the login button is clicked
    private fun signIn(email: String, password: String) {
        // sign_in_with_email
        auth.signInWithEmailAndPassword(email, password)
            .addOnCompleteListener(this) { task ->
                if (task.isSuccessful) {
                    // Sign in success, update UI with the signed-in user's information
                    if (detectEmailVerification()) {
                        Toast.makeText(baseContext, "Login Success", Toast.LENGTH_SHORT).show()

                        val userId = Firebase.auth.currentUser?.uid
                        verifyUserRoom(userId!!)

                        val intent = Intent(this@Login, Welcome::class.java)
                        startActivity(intent)
                        finish()
                    } else {
                        Toast.makeText(baseContext, "Email is not verified", Toast.LENGTH_SHORT)
                            .show()
                    }
                } else {
                    // If sign in fails, display a message to the user.
                    Toast.makeText(baseContext, "Login failed", Toast.LENGTH_SHORT).show()
                }
            }
    }

    // Check if the email is verified
    private fun detectEmailVerification(): Boolean {
        val user = auth.currentUser

        user?.let { currentUser ->
            val emailVerified = currentUser.isEmailVerified  // Return email verification status
            if (emailVerified) {
                return true
            }
        }
        return false
    }

    // Verifies if user data exists in Room database and fetches from Firestore if not
    private fun verifyUserRoom(userId: String) {
        lifecycleScope.launch(Dispatchers.IO) {
            val userData = Room.MyDatabase.invoke(applicationContext, userId)
                .userDao().getUser()

            if (userData == null) {
                db.collection("users").document(userId).get().addOnSuccessListener { document ->
                    if (document.exists()) {
                        // Fetch user data from Firestore
                        val idUser = document.getString("id")
                        val nGroup = document.getLong("groupid")
                        val user = document.getString("name")
                        val userEmail = document.getString("email")
                        val userPhone = document.getString("phone")
                        val userPhoto = document.getString("photo")

                        // Insert user data into Room database
                        lifecycleScope.launch(Dispatchers.IO) {
                            Room.MyDatabase.invoke(applicationContext, userId).userDao().insertAll(
                                Room.User(
                                    idUser = idUser!!, user = user!!, userEmail = userEmail!!,
                                    userPhone = userPhone!!, nGroup = nGroup!!, userPhoto = userPhoto!!
                                ))
                        }
                    }
                }
            }
        }
    }

    // Navigate to the registration page with email and password
    fun buttonEmailAndPasswordRegister(view: View) {
        val intent = Intent(this@Login, Register::class.java)
        startActivity(intent)
    }

    // Navigate to phone number registration page
    fun buttonPhoneResister(view: View) {
        val intent = Intent(this@Login, PhoneNumberVerification::class.java)
        startActivity(intent)
    }

    // Navigate to the forgot password page
    fun toForgotPasswordOnClickBtn(view: View) {
        val intent = Intent(this@Login, ForgotPassword::class.java)
        startActivity(intent)
    }

    @SuppressLint("MissingSuperCall")
    override fun onBackPressed() {
        // Disable back button functionality
        Toast.makeText(this@Login, "Back is destabilised", Toast.LENGTH_SHORT).show()
    }
}
