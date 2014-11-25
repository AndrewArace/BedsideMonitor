/*
 *  Bedside Monitor
 *
 *  Author: Andrew Arace
 *  Date: 2014-11-24
 *  Capabilities:
 *   Switch
 *  Custom Attributes:
 *   greeting
 *  Custom Commands
 *   hello
 *   goodbye
 */
 
 
metadata {
	definition (name: "Bedside Monitor", author: "Andrew Arace") {
    		capability "Switch"
        	command "hello"
        	command "goodbye"
        	attribute "greeting","string"
	}

	tiles {
		standardTile("switch", "device.switch", width: 2, height: 2, canChangeIcon: true, canChangeBackground: true) {
			state "on", label: '${name}', action: "switch.off", icon: "st.switches.switch.on", backgroundColor: "#79b821"
			state "off", label: '${name}', action: "switch.on", icon: "st.switches.switch.off", backgroundColor: "#ffffff"
		}
		standardTile("greeting", "device.greeting", width: 1, height: 1, canChangeIcon: true, canChangeBackground: true) {
			state "default", label: 'hello', action: "hello", icon: "st.switches.switch.off", backgroundColor: "#ccccff"
		}     
		valueTile("message", "device.greeting", inactiveLabel: false) {
			state "greeting", label:'${currentValue}', unit:""
		}
        
		main "switch"
		details(["switch","greeting","message"])
	}
    simulator {

    }
}

Map parse(String description) {
	log.debug "Map parse: '${description}'"
    
	def value = zigbee.parse(description)?.text
	def linkText = getLinkText(device)
	def descriptionText = getDescriptionText(description, linkText, value)
	def handlerName = value
	def isStateChange = value != "ping"
	def displayed = value && isStateChange

	def result = [
		value: value,
		name: value in ["on","off"] ? "switch" : (value && value != "ping" ? "greeting" : null),
		handlerName: handlerName,
		linkText: linkText,
		descriptionText: descriptionText,
		isStateChange: isStateChange,
		displayed: displayed
	]

	log.debug result.descriptionText
	result
}

def on() {
    zigbee.smartShield(text: "on").format()
}

def off() {
	zigbee.smartShield(text: "off").format()
}

def hello() {
	log.debug "Hello World!"
	zigbee.smartShield(text: "hello").format()
}

def goodbye() {
	log.debug "Bye Bye!"
	zigbee.smartShield(text: "goodbye").format()
}

def parse(String description) {
	log.debug "Raw input: '${description}'"
    log.debug "Payload is: " + zigbee.parse(description)?.text
	// TODO: handle 'switch' attribute
	// TODO: handle 'greeting' attribute
    def result = createEvent(name: "message", value: zigbee.parse(description)?.text as Double)
    log.debug result?.descriptionText
    return result

}