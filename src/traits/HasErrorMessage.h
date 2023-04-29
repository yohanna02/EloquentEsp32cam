//
// Created by Simone on 09/09/22.
//

#ifndef ELOQUENTESP32CAM_HASERRORMESSAGE_H
#define ELOQUENTESP32CAM_HASERRORMESSAGE_H


namespace Eloquent {
    namespace Esp32cam {
        /**
         * Log error messages
         */
        class HasErrorMessage {
        public:

            /**
             *
             * @return
             */
            String getErrorMessage() {
                return _error;
            }

            /**
             *
             * @return
             */
            bool isOk() {
                return _error == "";
            }

        protected:
            String _error;

            /**
             *
             * @param error
             */
            bool setErrorMessage(String error, const char *channel = "ERROR") {
                _error = error;

                if (error != "")
                    ESP_LOGE(channel, "%s", error.c_str());

                return isOk();
            }
        };
    }
}

#endif //ELOQUENTESP32CAM_HASERRORMESSAGE_H
